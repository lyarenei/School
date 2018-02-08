<?php

require_once 'Instruction.php';

class Program {

    private $__language;
    private $__instructions = array();

    public function setLanguage($language) {
        $this->__language = $language;
    }

    public function getLanguage() {
        return $this->__language;
    }

    public function insertInstruction($instructionLine) {
        static $counter = 1;
        $instruction = new Instruction();
        $instructionParams = preg_split('/\s+/', $instructionLine);
        $instruction->setOrder($counter);
        $instruction->setOpcode($instructionParams[0]);
        $instruction->setArguments($instructionParams);
        array_push($this->__instructions, $instruction);
        $counter++;
    }

    public function getInstructions() {
        return $this->__instructions;
    }
}

?>
