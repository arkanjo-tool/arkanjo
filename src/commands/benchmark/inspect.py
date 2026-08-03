import argparse
from dataclasses import dataclass
import subprocess
import json

import pyarrow.parquet as pq

from textual.app import App, ComposeResult
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.widgets import Footer, Header, RichLog, Static

from rich.columns import Columns
from rich.panel import Panel
from rich.padding import Padding
from rich.table import Table
from rich.text import Text

from rich.text import Text
from rich.ansi import AnsiDecoder

from .datasets.bigclonebench import BigCloneBenchAdapter

def clamp(v, min_v, max_v):
    return max(min_v, min(v, max_v))


def metric_color(value: float) -> str:
    if value >= 0.90:
        return "green"
    if value >= 0.70:
        return "yellow"
    return "red"

@dataclass
class InspectState:
    df: any
    arkanjo_names: list
    all_metrics: dict
    index: int = 0

    @property
    def row(self):
        if self.df.empty:
            return None
        return self.df.iloc[self.index]

    @property
    def size(self):
        return len(self.df)

    @property
    def aggregate(self):
        return self.all_metrics.get("aggregate", {})

class InspectApp(App):

    BINDINGS = [
        ("n", "next_row", "Next"),
        ("p", "prev_row", "Previous"),
        ("q", "quit", "Quit"),
    ]

    def __init__(self, config: argparse.Namespace, **kwargs):
        super().__init__(**kwargs)
        self.config = config
        self.state: InspectState | None = None

    def compose(self) -> ComposeResult:
        yield Header()
        rich_log = RichLog(id="diff", highlight=True, wrap=True)
        rich_log.styles.scrollbar_size_horizontal = 0
        yield Horizontal(
            VerticalScroll(
                Static(id="dashboard")
            ),
            rich_log,
        )
        yield Footer()

    def on_mount(self) -> None:
        pf = pq.ParquetFile(self.config.file)
        metadata = pf.schema_arrow.metadata

        arkanjo_names = json.loads(metadata[b"names"].decode("utf-8"))
        all_metrics = json.loads(metadata[b"metrics"].decode("utf-8"))

        df = pf.read().to_pandas()

        if self.config.result:
            df = df[df.result == self.config.result]

        self.state = InspectState(
            df=df,
            arkanjo_names=arkanjo_names,
            all_metrics=all_metrics,
        )

        self.refresh_ui()

    def refresh_ui(self):
        self.render_screen()
        self.render_diff()

    def render_screen(self):
        st = self.state
        row = st.row

        sample = Table.grid(padding=(0, 1))
        sample.add_row("Index", f"{st.index + 1} / {st.size}")
        sample.add_row("Result", str(row.result))
        sample.add_row("Type", str(row.syntactic_type))
        sample.add_row("Pair", f"{row.id1} ↔ {row.id2}")

        sample_panel = Panel(
            sample,
            title="Current Sample"
        )

        metrics = Table.grid(padding=(0, 1))
        metrics.add_column(justify="left")
        metrics.add_column(justify="right")

        for metric in ("f1", "precision", "recall", "accuracy"):
            value = st.aggregate.get(metric, 0.0)

            metrics.add_row(
                metric.upper(),
                Text(
                    f"{value:.1%}",
                    style=metric_color(value),
                ),
            )

        metrics_panel = Panel(
            metrics,
            title="Overall Performance"
        )

        top = Columns(
            [sample_panel, metrics_panel],
            expand=True,
        )

        rows = []

        for name, m in st.all_metrics.items():
            if name == "aggregate":
                continue

            tp = m.get("tp", 0)
            fn = m.get("fn", 0)
            fp = m.get("fp", 0)
            tn = m.get("tn", 0)

            recall = tp / (tp + fn) if (tp + fn) else 0
            precision = tp / (tp + fp) if (tp + fp) else 0
            f1 = (2 * precision * recall / (precision + recall)) if (precision + recall) else 0
            accuracy = (tp + tn) / (tp + tn + fp + fn) if (tp + tn + fp + fn) else 0

            rows.append((precision, recall, f1, accuracy, name))

        adapter = BigCloneBenchAdapter(self.config, None)
        rows.sort(key=lambda row: adapter.get_types_order().get(row[4], 999))

        # recall
        def create_table_metrics(value_str, index):
            table = Table(
                expand=True, box=None
            )

            table.add_column("Type")
            table.add_column(value_str.title(), justify="right")
            # recall_table.add_column("TP/Total", justify="right")

            for row in rows:
                precision, recall, f1, accuracy, name = row
                table.add_row(
                    name,
                    Text(f"{row[index]:.0%}", style=metric_color(row[index])),
                    # f"{tp}/{total}",
                )

            panel = Panel(
                table,
                title=f"{value_str.title()} by Type"
            )

            return panel
            

        dashboard = Table.grid(padding=(1, 1))
        dashboard.add_row(top)

        p1 = create_table_metrics('recall', 1)
        p2 = create_table_metrics('precision', 0)
        dashboard.add_row(Columns(
            [p1, p2],
            expand=True,
        ))
        p3 = create_table_metrics('f1', 2)
        p4 = create_table_metrics('accuracy', 3)
        dashboard.add_row(Columns(
            [p3, p4],
            expand=True,
        ))

        self.query_one("#dashboard", Static).update(
            Padding(dashboard, (1, 1))
        )
        self.render_diff()

    def render_diff(self):
        st = self.state
        row = st.row

        log = self.query_one("#diff", RichLog)
        log.clear()
        log.write(Text(f"{row.id1} ↔ {row.id2}", style="bold"))

        for name in st.arkanjo_names:
            result = subprocess.run(
                [
                    "arkanjo",
                    "git-diff",
                    "--name", name,
                    str(row.id1),
                    str(row.id2),
                ],
                capture_output=True,
                text=True,
            )

            if result.returncode == 0:
                for segment in AnsiDecoder().decode(result.stdout):
                    log.write(segment)
                return

    def action_next_row(self):
        st = self.state
        st.index = clamp(st.index + 1, 0, st.size - 1)
        self.render_screen()

    def action_prev_row(self):
        st = self.state
        st.index = clamp(st.index - 1, 0, st.size - 1)
        self.render_screen()

def main__inspect(args: argparse.Namespace, parser):
    InspectApp(config=args).run()