import pyRTOS
from wifi_module import connect_to_wifi
from shell_module import handle_client, server_task
from mailbox import temp_queue


def wifi_task(self):
    print("Wifi Task Starting")
    SSID = "Shad0wRun33rVX_2.4ghz"
    PASSWORD = "GRPhzsVx7R"
    payload = ["wifi", SSID, PASSWORD]
    yield [temp_queue.nb_send(payload)]


def init_task(self):
    print("Init Starting")
    yield


# Initialize pyRTOS

entryPoint = pyRTOS.Task(init_task, name="init_task")
wifiEntry = pyRTOS.Task(wifi_task, name="wifi_task")
wifiMan = pyRTOS.Task(connect_to_wifi, name="wifiman", mailbox=True)
shellServer = pyRTOS.Task(server_task, name="server_task", mailbox=True)
#cmdMan = pyRTOS.Task(handle_client, name="client_msg", mailbox=True)
wifiMan.deliver(temp_queue)
shellServer.deliver(temp_queue)
#cmdMan.deliver(temp_queue)
pyRTOS.add_task(entryPoint)
pyRTOS.add_task(wifiEntry)
pyRTOS.add_task(wifiMan)
pyRTOS.add_task(shellServer)
#pyRTOS.add_task(cmdMan)


pyRTOS.start()
