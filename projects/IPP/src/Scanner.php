<?php

require_once 'Logger.php';

define("FILE_ERROR_CODE", 11);

class Scanner {
    private $__sourceFile;

    private $__logger;

    public function __construct() {
        $this->__logger = new Logger();
    }

    public function openFile($fileName) {
        if (is_readable($fileName)) {
            $this->__sourceFile = fopen($fileName, "r");
        } else {
            $this->__logger->printError("Cannot open file: The specified file does not exist.", FILE_ERROR_CODE);
        }
    }

    public function getLine() {
        $line = fgets($this->__sourceFile);
        if ($line == FALSE) {
            return;
        }

        $line = preg_replace('/#.*/', '', $line);
        $line = trim($line, " \t\n\r\0\x0B");
        if (empty($line)) {
            return $this->getLine();
        } else {
            return $line;
        }
    }
}

?>
