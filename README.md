# Secure Kuznechik Messenger

Защищённый мессенджер с шифрованием по ГОСТ Р 34.12-2015 (Кузнечик).

## Description

Проект представляет собой клиент-серверное приложение для обмена зашифрованными сообщениями. Сервер работает как ретранслятор (не расшифровывает сообщения), обеспечивая end-to-end шифрование между клиентами.

### Сryptography
- **Алгоритм:** Кузнечик (ГОСТ Р 34.12-2015) — блочный шифр с размером блока 128 бит и ключом 256 бит.
- **Режим:** ECB (каждый блок шифруется независимо).
- **Дополнение:** PKCS#7 (дополнение до 16 байт).
- **Ключ:** 32-байтовый ключ, зашитый в клиенте (для демонстрации).

### Net
- **Транспорт:** TCP-сокеты.
- **Протокол:** Перед каждым сообщением передаётся 4-байтовая длина (big-endian).
- **Сервер:** Ретранслятор, пересылает зашифрованные данные между клиентами без расшифровки.

## Compilation:
```bash
gcc -o clie clie.c -lm
gcc -o serv serv.c -lm
```

## Launch:
```bash
./serv    # Terminal 1
./clie    # Terminal 2
./clie    # Terminal 3 or another computer's terminal 1
```

## Launch via the internet (Tailscale)
if u want your friend to be able to connect from any network:
```bash
# 1. install Tailscale: https://tailscale.com
# 2. in the client, replace 127.0.0.1 with the Tailscale server IP
# 3. start the server and share your Tailscale IP with your friend
```