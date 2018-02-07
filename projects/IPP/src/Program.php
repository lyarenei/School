<?php

require_once 'Instruction.php';

class Program {

    private $__language;
    private $__instructions = array();

    public function setProgramLanguage($language) {
        $this->__language = $language;
    }

    public function insertInstruction($instructionLine) {
        static $counter = 1;
        $instruction = new Instruction();
        $instructionParams = preg_split('/\s+/', $instructionLine);
        $instruction->setOrder($counter);
        $instruction->setOpcode($instructionParams[0]);
        $instruction->setInstructionArguments($instructionParams);
        array_push($this->__instructions, $instruction);
        $counter++;
    }
}

?>
