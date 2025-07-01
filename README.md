\page arkanjo_readme Arkanjo Module

The arkanjo is a cli tool is designed to help developers find code duplications in their codebases.
The tool is designed to find duplication in the scope of functions.

The current functionalities of the tool are:

- Explore the duplications on a codebases, enabling a limited amount of filters to the user.

- Find all functions that are a duplication of a function given by the user.

- Create a "relatory" of the number of duplications in the codebases separed by folder.

There are some other functionalities used for the cretor master degree, but should not matter for 
an end user. 

From now on, we will use the term commands instead of functionalities.

The tool current should cover the C programming language. 
The tool also supports Java with limitations.

# Similarity

The tool current uses the concept of similarity. The user can pass a similarity threshold
to the tool. This similarity threshold is used for limiting what the tool consider a duplication. 

The similarity threshold is a number between 0 and 100. If you set the threshold to 0, everything is
considered a duplication. If you set the threhold to 100, only complety equal functions are considered
duplications. The current state of the tool gives good results for similarity threshols around ~90.

The arkanjo tool uses as a subrotine the 
[Duplicate Code Detection Tool](https://github.com/platisd/duplicate-code-detection-tool) to generate
the similarity metrics.

# Requirements

The tool have only being tested on ubuntu operation systems. The instalation guide could included.

# How to install

Run on the terminal the following commands to install the dependencies:

```
sudo apt install python3-nltk
sudo apt install python3-gensim
sudo apt install python3-astor
sudo python3 -m nltk.downloader punkt
sudo apt-get install libjsoncpp-dev
```

Download the source code:

```
git clone https://github.com/LipArcanjo/arkanjo.git
```

Go to the tool folder:

```
cd arkanjo
```

Build the binary:

```
make preprocessor
make
```

# How to Run

## Preprocessor

The tool is designed to have a heavy preprocesing, to enable the tool to answer different kind of queries fastly.

To do the preprocessing, execute the preprocessor:

```
./preprocessor
```

The preprocessor will ask for the complete path to the codebase you are interested to find code duplications and
the similarity threshold.

The preprocessor can take a while to execute. Depending on the size of the codebases, it cannot run or take a some hours. 

## Execute the run

To execute the tool and the tool functionalities, you need to run the command that respect this format:

```
./exec command [command_parameters] [-pre] [-s <NUMBER>]
```

If you have not executed the preprocessor, the tool will automatically call the preprocessor.

The shared parameters over all commands are:

- [-p] forces the preprocessor to execute.

- [s \<NUMBER\>] changes the similarity threshould to NUMBER, but just for current command.

The following are the guide on how to execute the current commands of the tool:

### Explore duplications

To execute the explore duplications command, run:

```
./exec ex [-l <number>] [-p <string>] [-b <'T' or 'F'>] [-c 'T' or 'F'] 
```

All the positional parameters are optional, and the meaning of them are the following:

- [-l \<number\>] limits the number of results showed to the user. The tool shows all results by default.

- [-p \<string\>] defines a pattern that the function names should match to show in the result. A
function name is said to match the pattern if the function name has the pattern as a substring of the string 
formed by the concatenation of the file path that the function exist and the function name.

- [-b 'T' or 'F'] 'T' if the pattern need to match both of the functions in the comparation. 'F' if at least
one of the functions match the pattern. Default value is 'F'.

- [-c 'T' or 'F'] 'T' if the results should be sorted by the number of duplicated lines. 'F' if the results
should be sorted by the similarity metric. Default value is 'F'.

### Find function duplicates

To execute the relatory commands, execute:

```
./exec fu <FUNCTION_NAME>
```

Where \<FUNCTION_NAME\> is a parameter passed by the user. The tool will match
the parameter with any function that has the FUNCTION_NAME as a substring.

### Relatory

To execute the relatory commands, execute:

```
./exec du
```
