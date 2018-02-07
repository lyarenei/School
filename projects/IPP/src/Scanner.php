<?php

    require_once('Logger.php');

    class Scanner {
        private $sourceFile;

        private $logger;
        
        function __construct()
        {
            $this->logger = new Logger();
        }

        public function OpenFile($fileName) {
            if (is_readable($fileName)) {
                $this->sourceFile = fopen($fileName, "r");
            } 
            else {
                $this->logger->PrintError("Cannot open file: The specified file does not exist.", 1);
            }
        }
    }

?>
