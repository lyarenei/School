<?php

    require_once('./src/FileAnalyzer.php');

    function printHelp() {
        fwrite(STDERR, "Help is to be written\n");
        exit(0);
    }

    if (count($argv) == 0) {
        printHelp();
    }

    $fileName = 'test.txt';

    $fileAnalyzer = new FileAnalyzer();
    $fileAnalyzer->AnalyzeFile($fileName);

?>
