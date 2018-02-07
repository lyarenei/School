<?php

    require_once('Scanner.php');
    require_once('Program.php');

    class FileAnalyzer {

        private $scanner;

        function __construct()
        {
            $this->scanner = new Scanner();
        }

        public function AnalyzeFile($fileName) {
            $this->scanner->OpenFile($fileName);
            $program = new Program();
            $program->SetProgramLanguage($this->scanner->GetLine()); // remove leading dot, spaces, etc, no match == error?

            while (($line = $this->scanner->GetLine()) != '') {
                $program->InsertInstruction($line);
            }

            return $program;
        }
    }

?>
