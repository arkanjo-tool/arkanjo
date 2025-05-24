# Test Duplication Code

I want to create some test cases to check the ability to detect duplication code on C code. This repository worth is store this test cases and document my finds.

# About Parser

In fact none of the tools accomplished the expectations. 

The only useful one are https://github.com/platisd/duplicate-code-detection-tool. 

The parser's folder are tools create to do more complex detectetion code. 

Parser folder is about doing comparations in structs/functions scope, while file_parser is do comparations in files scope.

At the moment, the parser tools uses the above tool as a subrotine to not do more repetitive work. But it can be changed if necessary.

Both tools has their own REAMDE. Read them to learn more.

Probably it is good to create a new repository to the parser folder, but I think about it latter when the tool is more solid.

# List of Candidate Tools to Test

https://pmd.github.io/pmd/pmd_userdocs_cpd

- Only able to detect repetition of TOKEN when specify TOKEN size. 
- Installed and tested it. Prety simple tool.

https://cppdepend.com/blog/tracking-the-hidden-duplicate-code-in-a-c-code-base/

- It is a paid tool. Should I try it? I think not.
- There is a free trial.

https://github.com/dlidstrom/Duplo

- I like it. It is only able to detect repetition of TOKEN.
- For the tool. TOKEN is defined as a set of consecutive lines with a minimum size per line.
- The output is easy to undertand and usage is easy with docker.
- Prety simple tool.

https://simian.quandarypeak.com/

- It is a paid tool. Should I try it? I think not.
- There is a license for academic research and education use.

https://codeclimate.com/

- It is a paid tool. 
- It is free tool for open source projects. It is fully automate on a website
- Search through github repository.
- Do not check any .c file. Maybe I will try later with change extension to .cpp.


https://www.researchgate.net/publication/2866183_Visual_Detection_of_Duplicated_Code

- Duploc 
- There is a pape about that. Looks cool.
- Unfortunaly, I was not able to find an source to test the tool.
- Main source link on paper is broken.

https://github.com/duploq/duploq

- It is a frond end aplication for https://github.com/dlidstrom/Duplo
- Look like it is easy to configurate.
- I will not install it for know. Since it can only do what Duplo can do. 

https://github.com/platisd/duplicate-code-detection-tool

- As a first impression, it is a amazing tool.
- Check similarity through different files and gives back a porcentage of similarity between each pair of files.
- It is easy to install and use. 
- Support GitHub Action. Looks easy to do CI/CD with it.
- Do not check similarity on code of the same file. I.e, the tool cannot check a copy-paste code if the copy-paste happened on the same file.
