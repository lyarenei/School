<?php

    class Argument {
        private $type;
        
        public function SetArgumentType($type) {
            $this->type = $type;
        }

        public function GetArgumentType() {
            return $type;
        }
    }

?>
