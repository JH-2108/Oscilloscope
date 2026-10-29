# Oscilloscope
A compact, low-cost digital oscilloscope designed to visualize electrical signals in real time using a microcontroller and a display. 

The goal of this project is to build an affordable educational instrument that can sample low-voltage signals, displaying their waveforms, and provide a simple interface for observing basic electrical behavior. 

# Features 
- Real-time waveform visualization on a display
- Microcontroller-based analogue signal sampling
- voltage and time-scale indicators
- triggering for a more stable waveform display
- low-cost, accessible electronic components
- compact design with the potential for a custom enclosure

# Project goals 
- Develop a functional signal acquisition and waveform display system
- create a clear, usable interface for viewing electrical signals.
- document the entire process, check [journal](journal.md) to see it!

# Planned hardware components
| Component | Purpose |
|---|---|
| Microcontroller development board | Samples and processes the input signal | 
| Color display module | displays the waveforms and measurement information |
| input protection and conditioning circuit | Keeps input signals within safe operating limits |
| passive components | support signal conditioning and circuit operation | 
| user controls | adjust display and acquisition settings |
| prototyping hardware | supports circuit development and testing | 

# How this oscilloscope would work 
1. An input circuit receives a low-voltage electrical signal
2. The microcontroller samples the signal using its analogue-to-digital converter (ADC).
3. The firmware processes the samples and determines where to begin drawing the waveform.
4. The display renders the waveform against a time and voltage grid.
5. User controls adjust the viewing and acquisition settings where supported.

# Bill of Materials 

Will be inserted later

# Project pictures
**PCB design** 

<img width="785" height="466" alt="image" src="https://github.com/user-attachments/assets/d6af0faa-8821-4f1c-8b6f-bf1bd93c5277" />

**Schematics**

<img width="597" height="680" alt="image" src="https://github.com/user-attachments/assets/2c802447-aab4-432c-b8ed-ef5470ac361b" />

**3D renderred PCB design**

<img width="895" height="535" alt="image" src="https://github.com/user-attachments/assets/781b1ff3-a14f-4a5d-a33f-c8fcef60af5d" />

**3D render with enclosure**

A picture will be added here 



