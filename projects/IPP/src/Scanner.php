<?php

require_once 'Logger.php';

define("FILE_ERROR_CODE", 11);

class Scanner {
    private $__sourceFile;

    private $__logger;

    public function __construct() {
        $this->__logger = new Logger();
    }

    private function __parseLine($line) {
        $trimmedLine = trim($line, " \t\n\r\0\x0B");
        if (empty($trimmedLine)) {
            return FALSE;
        }
        return $trimmedLine;
    }

    public function openFile($fileName) {
        if (is_readable($fileName)) {
            $this->__sourceFile = fopen($fileName, "r");
        } else {
            $this->__logger->printError("Cannot open file: The specified file does not exist.", FILE_ERROR_CODE);
        }
    }

    public function getLine() {
        do {
            $line = fgets($this->__sourceFile);
            $lineT = preg_split('/#.*/i', $line);
            if ($this->__parseLine($lineT[0]) == FALSE) {
                if ($line == FALSE) {
                    return;
                } else {
                    continue;
                }
            } else {
                return $lineT[0];
            }
        } while ($line != FALSE);
    }
}

?>
