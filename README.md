# 📈 PWM Drawer — Mini Oscilloscope

A mini software oscilloscope built on AVR (ATmega32) with a graphical LCD that visualizes PWM waveforms in real time, displaying frequency, duty cycle, and cycle time — from both internal and external signal sources.

---

## 📌 Project Overview

The PWM Drawer is a standalone embedded instrument that captures a PWM signal and renders its waveform on a graphical LCD. It measures and displays the signal's frequency (kHz), duty cycle (%), and period (time of one full cycle). It accepts signals from an external source (e.g. another microcontroller) or generates and measures its own internal PWM signal.

---

## ✨ Features

- 📊 **Real-time waveform rendering** on a graphical LCD
- 📡 **Dual input mode** — internal PWM source or external signal input
- 🔢 **Frequency display** — shown in kHz (upper-left of LCD)
- 💯 **Duty cycle display** — shown as % (upper-right of LCD)
- ⏱️ **Period display** — time of a single cycle shown on LCD
- ⚡ **Fast capture** — uses Timer Input Capture for accurate measurement

---

## 🗂️ System Architecture

```
  External Signal ──────────────────────┐
                                        ▼
  Internal Timer (PWM Gen) ──────► ATmega32 MCU
                                        │
                              ┌─────────┴──────────┐
                              │  Signal Analysis:   │
                              │  • Frequency (kHz)  │
                              │  • Duty Cycle (%)   │
                              │  • Period (µs/ms)   │
                              └─────────┬──────────┘
                                        │
                                        ▼
                               Graphical LCD Display

  ┌────────────────────────────────────────┐
  │  Frequency: XX.X kHz  DutyCycle: XX%  │
  │                                        │
  │   ┌──┐    ┌──┐    ┌──┐    ┌──┐       │
  │   │  │    │  │    │  │    │  │       │  ← Waveform
  │ ──┘  └────┘  └────┘  └────┘  └──    │
  │                                        │
  │  Time: XX.X µs                        │
  └────────────────────────────────────────┘
```

---

## ⚙️ Signal Measurement Method

| Parameter | Measurement Technique |
|---|---|
| **Frequency** | Timer counting between two rising edges (T = 1/f) |
| **Duty Cycle** | HIGH time / Total period × 100% |
| **Period** | Full cycle duration via Input Capture Unit |

### Input Capture Workflow
1. Configure Timer1 in Input Capture mode
2. Detect rising edge → record timestamp T1
3. Detect falling edge → record timestamp T2 (HIGH duration)
4. Detect next rising edge → record timestamp T3 (full period)
5. Calculate: `Period = T3 - T1`, `Duty = (T2 - T1) / Period × 100`

---

## 📟 LCD Display Layout

```
┌──────────────────────────────────┐
│ Freq: 1.23 kHz   Duty: 65%      │
│                                  │
│  ┌──────┐   ┌──────┐   ┌──      │
│  │      │   │      │   │        │
│──┘      └───┘      └───┘        │
│                                  │
│ Time: 813 µs                    │
└──────────────────────────────────┘
```

---

## 🧰 Components

| Component | Model | Purpose |
|---|---|---|
| Microcontroller | ATmega32 | Signal capture & processing |
| Graphical LCD | 128×64 (e.g. KS0108) | Waveform + data display |
| Signal Input | External pin / Internal Timer | PWM source |
| Crystal Oscillator | 16 MHz | Accurate timing reference |
| Capacitors + Resistors | — | Power filtering & signal conditioning |

---

## 🔌 Interfaces Used

- **Timer1 Input Capture Unit** — Precise edge detection for frequency & duty cycle
- **Timer0 / Timer2 (PWM)** — Internal signal generation mode
- **SPI or parallel interface** — Graphical LCD communication
- **GPIO** — LCD control signals

---

## 🛠️ Tools & Environment

- **Language:** Embedded C
- **MCU:** ATmega32 (AVR)
- **Simulation:** Proteus
- **Programmer:** AVRDUDE / USBasp

---

## 🔭 Future Improvements

- [ ] Add adjustable time-per-division (zoom in/out on waveform)
- [ ] Support multiple waveform types (sine approximation, triangle)
- [ ] Add min/max voltage display via ADC
- [ ] Store waveform snapshots to EEPROM
- [ ] Port to STM32 with a TFT display for higher resolution

---

## 👥 Team

Project built as part of the **Embedded Systems Diploma** at AMIT Learning.

---

## 📄 License

This project is for educational purposes.
