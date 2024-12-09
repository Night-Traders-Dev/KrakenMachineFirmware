import pyRTOS
temp_queue = pyRTOS.MessageQueue(capacity=10)
temp_buffer = []
mailbox = []
def mailbox_add(mail):
    mail.append("FINISHED")
    if mail not in mailbox:
        mailbox.append(mail)
        print(mail)
    else:
        print(mailbox)
        yield