import gc
import board
import microcontroller
import os
import wifi
import socketpool
import pyRTOS

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



def server_task(self):
    """Task to handle incoming connections."""
    print("Server Task Starting")
    PORT = 23
    server_socket = None
    client_socket = None
    buffer = bytearray(BUFFER_SIZE)
    reboot_flag = False
    exit_flag = False
    yield

    while True:
        # Step 1: Wait for Wi-Fi connection
        if wifi.radio.ipv4_address is None:
            print("Waiting for Wi-Fi connection...")
            yield [pyRTOS.timeout(2)]  # Wait for 2 seconds

        # Step 2: Initialize the server socket
        elif server_socket is None:
            print("Initializing server socket...")
            try:
                pool = socketpool.SocketPool(wifi.radio)
                server_socket = pool.socket(pool.AF_INET, pool.SOCK_STREAM)
                server_socket.setsockopt(pool.SOL_SOCKET, pool.SO_REUSEADDR, 1)
                server_socket.bind(("0.0.0.0", PORT))
                server_socket.listen(1)
                print(f"Server listening on port {PORT}")
            except Exception as e:
                print(f"Error initializing server socket: {e}")
                server_socket.listen(1)
                print(f"Server listening on port {PORT}")
                yield [pyRTOS.timeout(2)]
        
        # Step 3: Accept a client connection
        elif client_socket is None:
            print("Waiting for a client connection...")
            try:
                client_socket, addr = server_socket.accept()
                print(f"Accepted connection from {addr}")
                client_socket.send(b"Welcome to the shell!\nType 'help' for commands.\n$ ")
            except Exception as e:
                print(f"Error accepting client connection: {e}")
                client_socket = None
                yield [pyRTOS.timeout(2)]
        
        # Step 4: Handle client commands
        elif client_socket:
            try:
                bytes_received = client_socket.recv_into(buffer)
                if bytes_received == 0:  # Client disconnected
                    print("Client disconnected.")
                    client_socket.close()
                    client_socket = None
                else:
                    data = buffer[:bytes_received].decode("utf-8").strip()
                    print(f"Command received: {data}")
                    
                    # Process the command
                    response, reboot_flag, exit_flag = handle_command(data)
                    client_socket.send(response.encode("utf-8"))
                    
                    # Handle reboot or exit
                    if reboot_flag:
                        microcontroller.reset()
                    elif exit_flag:
                        print("Closing connection.")
                        client_socket.close()
                        client_socket = None
            except Exception as e:
                print(f"Error handling client: {e}")
                if client_socket:
                    client_socket.close()
                client_socket = None

        # Allow other tasks to run
        yield
