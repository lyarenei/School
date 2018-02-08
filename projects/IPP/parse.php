<?php

require_once './src/FileAnalyzer.php';
require_once './src/Writer.php';

function printHelp() {
    fwrite(STDERR, "Help is to be written\n");
    exit(0);
}

if (count($argv) == 0) {
    printHelp();
}

$fileName = 'test.txt';

$fileAnalyzer = new FileAnalyzer();
$analyzedProgram = $fileAnalyzer->analyzeFile($fileName);

$writer = new Writer();
$writer->write($analyzedProgram, STDOUT);

?>
