<?php

    require_once('Instruction.php');

    class Program {

        private $language;
        private $instructions = array();

        public function SetProgramLanguage($language) {
            $this->language = $language;
        }

        public function InsertInstruction($instructionLine) {
            static $counter = 1;
            $instruction = new Instruction();
            $instructionParams = preg_split('/\s+/', $instructionLine);
            $instruction->SetOrder($counter);
            $instruction->SetOpcode($instructionParams[0]);
            $instruction->SetInstructionArguments($instructionParams);
            array_push($this->instructions, $instruction);
            $counter++;
        }
    }

?>
