<?php

    require_once('Logger.php');

    define("FILE_ERROR_CODE", 11);

    class Scanner {
        private $sourceFile;

        private $logger;
        
        function __construct()
        {
            $this->logger = new Logger();
        }

        private function ParseLine($line) {
            $trimmedLine = trim($line, " \t\n\r\0\x0B");
            $opcode = explode('#', $trimmedLine);
            if (empty($opcode[0])) {
                return FALSE;
            }
            return $opcode[0];
        }

        public function OpenFile($fileName) {
            if (is_readable($fileName)) {
                $this->sourceFile = fopen($fileName, "r");
            } 
            else {
                $this->logger->PrintError("Cannot open file: The specified file does not exist.", FILE_ERROR_CODE);
            }
        }

        public function GetLine() {
            do {
                $line = fgets($this->sourceFile);
                $opcode = $this->ParseLine($line);
                if ($opcode == FALSE || empty($opcode)) {
                    continue;
                } else {
                    return $opcode;
                }
            } while ($line != FALSE);
        }
    }

?>
