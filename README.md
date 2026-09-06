# Secure Kuznechik Messenger

Защищённый мессенджер с шифрованием по ГОСТ Р 34.12-2015 (Кузнечик).

## Описание

Проект представляет собой клиент-серверное приложение для обмена зашифрованными сообщениями. Сервер работает как ретранслятор (не расшифровывает сообщения), обеспечивая end-to-end шифрование между клиентами.

### Криптография
- **Алгоритм:** Кузнечик (ГОСТ Р 34.12-2015) — блочный шифр с размером блока 128 бит и ключом 256 бит.
- **Режим:** ECB (каждый блок шифруется независимо).
- **Дополнение:** PKCS#7 (дополнение до 16 байт).
- **Ключ:** 32-байтовый ключ, зашитый в клиенте (для демонстрации).

### Сеть
- **Транспорт:** TCP-сокеты.
- **Протокол:** Перед каждым сообщением передаётся 4-байтовая длина (big-endian).
- **Сервер:** Ретранслятор, пересылает зашифрованные данные между клиентами без расшифровки.

## compilation:
```bash
gcc -o clie clie.c -lm
gcc -o serv serv.c -lm
```

## launch:
```bash
./serv    # Terminal 1
./clie    # Terminal 2
./clie    # Terminal 3 or another computer's terminal 1
```

## launch via the internet (Tailscale)
if u want your friend to be able to connect from any network:
```bash
# 1. install Tailscale: https://tailscale.com
# 2. in the client, replace 127.0.0.1 with the Tailscale server IP
# 3. start the server and share your Tailscale IP with your friend
```
