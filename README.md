# SecretsOfElka
A game designed for TIN.

## Server changelog
### 0.2
- Backspace doesn't get added as separate character
- Dirty client disconnect is supported

### 0.3
- Server SO_REUSEPORT used to overcome EADDRINUSE
- bzero() not in use anymore
- Server can send and receive messages of any length
- Working on strings instead of char[] where possible