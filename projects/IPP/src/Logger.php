<?php

class Logger {

    public function printWarning($message) {
        fwrite(STDERR, "Warning: " . $message . "\n");
    }

    public function printError($message, $exitCode) {
        fwrite(STDERR, "Error: " . $message . "\n");
        exit($exitCode);
    }

}

?>
