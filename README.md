# simpl-messenger

simple messenger with small bug)

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
if u want your friends to be able to connect from any network:
```bash
# 1. install Tailscale: https://tailscale.com
# 2. in the client, replace 127.0.0.1 with the Tailscale server IP
# 3. start the server and share your Tailscale IP with your friends
```
