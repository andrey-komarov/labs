#!/bin/bash
alias antlr4='java -jar /home/andrey/lib/antlr-4.0-complete.jar'
antlr4='java -jar /home/andrey/lib/antlr-4.0-complete.jar'
mkdir -p build
cp TeX.g4 ToHtml.java Main.java build
cd build
$antlr4 -no-visitor -listener -encoding utf8 -o . TeX.g4
javac *.java
cp ../report.tex .
java Main < report.tex > report.html
mkdir -p ../out
cp report.html ../out
cp ../report-main.tex .
pdflatex report-main.tex
cp report-main.pdf ../out/report.pdf
cd ..
