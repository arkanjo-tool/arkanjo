class DatasetAdapter:
    def __init__(self, config, args, parser):
        raise NotImplementedError

    def get_test_dataset(self, ds):
        raise NotImplementedError
    
    def build_ground_truth(self, ds):
        raise NotImplementedError
    
    def create_batch_iter(self, workdir, row):
        raise NotImplementedError

    def get_syntactic_type(self, pair):
        raise NotImplementedError

    def get_types_order(self):
        raise NotImplementedError
