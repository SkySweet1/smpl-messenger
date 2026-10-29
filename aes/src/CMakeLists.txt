cmake_minimum_required(VERSION 3.10)
project(AesMessenger C)

# Устанавливаем стандарт C99 (так как используется объявление переменных внутри циклов)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Собираем клиент (нужны aes.c и network.c)
add_executable(clie clie.c aes.c network.c)

# Собираем сервер (нужен только network.c, AES ему не нужен)
add_executable(serv serv.c network.c)

# Линкуем математическую библиотеку (для sin, M_PI в rainbow_text_smooth)
target_link_libraries(clie m)
target_link_libraries(serv m)