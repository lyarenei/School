<?php

    require_once('Checker.php');

    class Instruction {

        private $order;
        private $opcode;

        private $arguments = array();

        public function SetOrder($order) {
            $this->order = $order;
        }

        public function SetOpcode($opcode) {
            $this->opcode = $opcode;
        }
    }

?>
