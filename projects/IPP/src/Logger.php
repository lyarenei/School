<?php

    class Logger {

        public function PrintWarning($message) {
            fwrite(STDERR, "Warning: " . $message . "\n");
        }

        public function PrintError($message, $exitCode) {
            fwrite(STDERR, "Error: " . $message . "\n");
            exit($exitCode);
        }

    }

?>
