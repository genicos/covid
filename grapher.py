import matplotlib.pyplot as plt
import math
import os

def create_graph(fips_number):
    try:
        file = open("counties/." + str(fips_number), "rb")
    except:
        return
    days = int.from_bytes(file.read(4), "little");
    

    name = bytearray(file.read(15))
    
    for i in range(len(name)):
        if name[i] == 0:
            name = name[:i]
            break
    
    
    name = name.decode("utf-8")

    state = bytearray(file.read(15))
    
    for i in range(len(state)):
        if state[i] == 0:
            state = state[:i]
            break
    
    state = state.decode("utf-8")

    
    cases       = [None] * days
    deaths      = [None] * days
    growth_rate = [None] * days
    log_cases   = [None] * days
    
    for i in range(days):
        cases[i] = int.from_bytes(file.read(4), "little")
        
    for i in range(days):
        deaths[i] = int.from_bytes(file.read(4), "little")
    
    for i in range(days):
        growth_rate[i] = int.from_bytes(file.read(4), "little")
    
    for i in range(days):
        log_cases[i] = math.log(cases[i] if cases[i] > 0 else 1,2)
    
    if not os.path.exists("graphs/"+str(fips_number)):
        os.mkdir("graphs/" + str(fips_number))
    
    plt.clf()
    plt.plot(range(days),cases)
    plt.savefig("graphs/"+str(fips_number)+"/cases.png")
    
    plt.clf()
    plt.plot(range(days),deaths)
    plt.savefig("graphs/"+str(fips_number)+"/deaths")

    plt.clf()
    plt.plot(range(days),growth_rate)
    plt.savefig("graphs/"+str(fips_number)+"/growth_rate")

    plt.clf()
    plt.plot(range(days),log_cases)
    plt.savefig("graphs/"+str(fips_number)+"/log_cases")

if not os.path.exists("graphs"):
    os.mkdir("graphs")

print("Loading...%")
for h in range(100):
    for i in range(570):
        create_graph(h*570 + i)
    print(h)

