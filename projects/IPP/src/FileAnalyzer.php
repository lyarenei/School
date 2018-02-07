<?php

require_once 'Scanner.php';
require_once 'Program.php';

class FileAnalyzer {

    private $__scanner;

    public function __construct() {
        $this->__scanner = new Scanner();
    }

    public function analyzeFile($fileName) {
        $this->__scanner->openFile($fileName);
        $program = new Program();
        $program->setProgramLanguage($this->__scanner->getLine()); // TODO: remove leading dot, spaces, etc, no match == error?

        while (($line = $this->__scanner->getLine()) != '') {
            $program->insertInstruction($line);
        }

        return $program;
    }
}

?>
