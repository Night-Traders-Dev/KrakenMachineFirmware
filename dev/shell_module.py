import gc
import board
import microcontroller
import os
import wifi
import socketpool
import pyRTOS
from mailbox import temp_queue, temp_buffer
import time

BUFFER_SIZE = 1024

def handle_command(command):
    """Process the user command."""
    command = command.strip().lower()
    reboot_flag = False
    exit_flag = False

    if command == "ssid":
        SSID = os.getenv('CIRCUITPY_WIFI_SSID')
        response = f"SSID: {SSID}\n"
    elif command == "ipaddr":
        response = f"IP Address: {wifi.radio.ipv4_address}\n"
    elif command == "reboot":
        response = "Rebooting system...\n"
        reboot_flag = True
    elif command == "exit":
        response = "Closing connection. Goodbye!\n"
        exit_flag = True
    elif command == "help":
        response = (
            "Available commands:\n"
            "- ssid: Show the connected Wi-Fi SSID\n"
            "- ipaddr: Show the device's IP address\n"
            "- reboot: Reboot the system\n"
            "- exit: Exit the shell and close the connection\n"
            "- temp: Show the microcontroller temperature\n"
            "- mem: Display memory usage\n"
            "- pins: List available board pins\n"
            "- env: Display environment variables\n"
            "- board: Show board-specific information\n"
            "- cpu: Show microcontroller details\n"
            "- help: Show this help message\n"
        )
    elif command == "temp":
        response = f"Microcontroller temperature: {microcontroller.cpu.temperature:.2f} °C\n"
    elif command == "mem":
        response = f"Memory Info:\nFree Memory: {gc.mem_free()} bytes\nAllocated Memory: {gc.mem_alloc()} bytes\n"
    elif command == "pins":
        response = "Available pins:\n"
        for pin in dir(board):
            response += f"  {pin}\n"
    elif command == "env":
        response = "Environment Variables:\n"
        for key, value in os.environ.items():
            response += f"  {key} = {value}\n"
    elif command == "board":
        response = f"Board Information:\nID: {board.board_id}\nUnique ID: {microcontroller.cpu.uid.hex()}\n"
    elif command == "cpu":
        response = f"CPU Information:\nFrequency: {microcontroller.cpu.frequency / 1_000_000:.2f} MHz\nTemperature: {microcontroller.cpu.temperature:.2f} °C\n"
    else:
        response = "Unknown command. Type 'help' for available commands.\n"

    return response, reboot_flag, exit_flag

def handle_client(client_socket):
    buffer = bytearray(BUFFER_SIZE)
    reboot_flag = False
    exit_flag = False
    client_socket.send(b"Welcome to the shell!\nType 'help' for commands.\n$ ")
    while not reboot_flag and not exit_flag:
        # Receive data from client
        bytes_received = client_socket.recv_into(buffer)
        if bytes_received == 0:  # No more data; client disconnected
            break

        # Extract the actual data received
        data = buffer[:bytes_received]
        print(f"Raw data received: {data}")

        # Assume plaintext for debugging
        decrypted_data = data.decode("utf-8").strip()
        print(f"Command received: {decrypted_data}")

        # Handle the command
        response, reboot_flag, exit_flag = handle_command(decrypted_data)
        client_socket.send(response.encode("utf-8"))
        if not reboot_flag and not exit_flag:
            client_socket.send(b"> ")
            gc.collect()
            yield
        if reboot_flag:
            microcontroller.reset()  # Reboot the system



def server_task(self):
    """Task to handle incoming connections."""
    print("Server Task Starting")
    PORT = 23
    server_socket = None
    yield
    while True:
        print("server_task waiting for msg")
        if wifi.radio.ipv4_address is None:
            print("pass")
            yield [pyRTOS.timeout(2)]
        elif server_socket is None and wifi.radio.ipv4_address:
            yield [temp_queue.recv(temp_buffer), pyRTOS.timeout(5)]
            payload = temp_buffer.pop()
            print(f"server:{temp_buffer}{payload}")
            if payload[0] == "server" and payload[2] == "start":
                connection = wifi.radio
                pool = socketpool.SocketPool(connection)
                server_socket = pool.socket(pool.AF_INET, pool.SOCK_STREAM)

                # Bind to the port and start listening
                server_socket.bind(("0.0.0.0", PORT))
                server_socket.listen(1)
                print(f"Server listening on port {PORT}")
                while True:
                    print("Waiting for a client...")
                    client_socket, addr = server_socket.accept()
                    print(f"Accepted connection from {addr}")
                    handle_client(client_socket)

            