<?php

    class Writer {
        private $outputFile;
        private $xmlWriter;

        private function __construct() {
            $xmlWriter = new XMLWriter();
        }

        public function Write($outputFile) {
            // TBD
        }
    }

?>