<?php

    require_once('Instruction.php');

    class Program {

        private $language;
        private $instructions = array();

        public function SetProgramLanguage($language) {
            $this->language = $language;
        }
    }

?>
