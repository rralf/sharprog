EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328P-AU U1
U 1 1 5F76EBF2
P 2550 2700
F 0 "U1" H 2550 1111 50  0000 C CNN
F 1 "ATmega328P-AU" H 2550 1020 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 2550 2700 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 2550 2700 50  0001 C CNN
	1    2550 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x11_Male J1
U 1 1 5F76A8F2
P 10650 1350
F 0 "J1" H 10758 2031 50  0000 C CNN
F 1 "Conn_01x11_Male" H 10758 1940 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x11_P2.54mm_Horizontal" H 10650 1350 50  0001 C CNN
F 3 "~" H 10650 1350 50  0001 C CNN
	1    10650 1350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10450 850  10250 850 
Text GLabel 10250 850  0    50   3State ~ 0
MTOUT2
$Comp
L power:+6V #PWR0101
U 1 1 5F778013
P 9100 850
F 0 "#PWR0101" H 9100 700 50  0001 C CNN
F 1 "+6V" H 9115 1023 50  0000 C CNN
F 2 "" H 9100 850 50  0001 C CNN
F 3 "" H 9100 850 50  0001 C CNN
	1    9100 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 850  9100 950 
Wire Wire Line
	9100 950  10450 950 
$Comp
L Diode:1N4148 D2
U 1 1 5F77CFE5
P 9100 1350
F 0 "D2" V 9054 1430 50  0000 L CNN
F 1 "1N4148" V 9145 1430 50  0000 L CNN
F 2 "Diode_SMD:D_MiniMELF" H 9100 1175 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 9100 1350 50  0001 C CNN
	1    9100 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	8700 1050 8900 1050
Wire Wire Line
	8700 1150 8700 1050
$Comp
L power:GND #PWR0102
U 1 1 5F778BD2
P 8700 1150
F 0 "#PWR0102" H 8700 900 50  0001 C CNN
F 1 "GND" H 8622 1113 50  0000 R CNN
F 2 "" H 8700 1150 50  0001 C CNN
F 3 "" H 8700 1150 50  0001 C CNN
	1    8700 1150
	1    0    0    -1  
$EndComp
Text GLabel 10250 1850 0    50   BiDi ~ 0
IB5
Text GLabel 10250 1750 0    50   BiDi ~ 0
IB6
Text GLabel 10250 1650 0    50   BiDi ~ 0
IB7
Text GLabel 10250 1550 0    50   BiDi ~ 0
IB8
Text GLabel 7000 1850 0    50   Output ~ 0
XOUT
Text GLabel 7000 1600 0    50   Input ~ 0
XIN
Text GLabel 10250 1250 0    50   BiDi ~ 0
F02
Text GLabel 10250 1150 0    50   BiDi ~ 0
F01
Wire Wire Line
	10450 1850 10250 1850
Wire Wire Line
	10450 1750 10250 1750
Wire Wire Line
	10450 1650 10250 1650
Wire Wire Line
	10450 1550 10250 1550
Wire Wire Line
	10450 1250 10250 1250
Wire Wire Line
	10450 1150 10250 1150
Wire Wire Line
	9100 1500 9100 1600
Wire Wire Line
	9100 1200 9100 950 
Connection ~ 9100 950 
$Comp
L Diode:1N4148 D1
U 1 1 5F789DC9
P 8900 1350
F 0 "D1" V 8946 1430 50  0000 L CNN
F 1 "1N4148" V 8855 1430 50  0000 L CNN
F 2 "Diode_SMD:D_MiniMELF" H 8900 1175 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 8900 1350 50  0001 C CNN
	1    8900 1350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10450 1350 9250 1350
Wire Wire Line
	9250 1350 9250 1600
Wire Wire Line
	9250 1600 9100 1600
Connection ~ 9100 1600
Wire Wire Line
	8900 1500 8900 1600
Wire Wire Line
	8900 1600 9100 1600
Wire Wire Line
	8900 1050 8900 1200
Connection ~ 8900 1050
Wire Wire Line
	8900 1050 10450 1050
$Comp
L Device:C C1
U 1 1 5F79003D
P 8450 1600
F 0 "C1" V 8198 1600 50  0000 C CNN
F 1 "100n" V 8289 1600 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8488 1450 50  0001 C CNN
F 3 "~" H 8450 1600 50  0001 C CNN
	1    8450 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 1600 8900 1600
Connection ~ 8900 1600
$Comp
L Device:R R1
U 1 1 5F793AC4
P 9850 1450
F 0 "R1" V 9965 1450 50  0000 C CNN
F 1 "330k" V 10056 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 9780 1450 50  0001 C CNN
F 3 "~" H 9850 1450 50  0001 C CNN
	1    9850 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 1450 10450 1450
$Comp
L Device:C C2
U 1 1 5F795460
P 8450 1850
F 0 "C2" V 8610 1850 50  0000 C CNN
F 1 "100n" V 8701 1850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8488 1700 50  0001 C CNN
F 3 "~" H 8450 1850 50  0001 C CNN
	1    8450 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 1850 9400 1450
Wire Wire Line
	9400 1450 9700 1450
Wire Wire Line
	8600 1850 9400 1850
$Comp
L Device:R R3
U 1 1 5F7972D6
P 9400 2100
F 0 "R3" H 9330 2054 50  0000 R CNN
F 1 "10k" H 9330 2145 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 9330 2100 50  0001 C CNN
F 3 "~" H 9400 2100 50  0001 C CNN
	1    9400 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	9400 1850 9400 1950
Connection ~ 9400 1850
$Comp
L power:GND #PWR0103
U 1 1 5F79865F
P 9400 2400
F 0 "#PWR0103" H 9400 2150 50  0001 C CNN
F 1 "GND" H 9405 2227 50  0000 C CNN
F 2 "" H 9400 2400 50  0001 C CNN
F 3 "" H 9400 2400 50  0001 C CNN
	1    9400 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 2400 9400 2250
$Comp
L Interface_USB:FT232RL U2
U 1 1 5F7894AE
P 2500 6250
F 0 "U2" H 2500 7431 50  0000 C CNN
F 1 "FT232RL" H 2500 7340 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 3600 5350 50  0001 C CNN
F 3 "https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232R.pdf" H 2500 6250 50  0001 C CNN
	1    2500 6250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 5F78D77C
P 2600 4900
F 0 "#PWR0104" H 2600 4750 50  0001 C CNN
F 1 "+5V" H 2615 5073 50  0000 C CNN
F 2 "" H 2600 4900 50  0001 C CNN
F 3 "" H 2600 4900 50  0001 C CNN
	1    2600 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4900 2600 5050
Wire Wire Line
	2600 5050 2400 5050
Wire Wire Line
	2400 5050 2400 5250
Connection ~ 2600 5050
Wire Wire Line
	2600 5050 2600 5250
Wire Wire Line
	1700 6250 1500 6250
Wire Wire Line
	1500 6250 1500 6200
$Comp
L power:+5V #PWR0105
U 1 1 5F78FB01
P 1500 6200
F 0 "#PWR0105" H 1500 6050 50  0001 C CNN
F 1 "+5V" H 1515 6373 50  0000 C CNN
F 2 "" H 1500 6200 50  0001 C CNN
F 3 "" H 1500 6200 50  0001 C CNN
	1    1500 6200
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B_Micro J2
U 1 1 5F79006F
P 800 5850
F 0 "J2" H 857 6317 50  0000 C CNN
F 1 "USB_B_Micro" H 857 6226 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Molex-105017-0001" H 950 5800 50  0001 C CNN
F 3 "~" H 950 5800 50  0001 C CNN
	1    800  5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 5950 1700 5950
Wire Wire Line
	1700 5850 1100 5850
Wire Wire Line
	1100 5650 1200 5650
Wire Wire Line
	1200 5650 1200 5500
$Comp
L power:+5V #PWR0106
U 1 1 5F7935B2
P 1200 5500
F 0 "#PWR0106" H 1200 5350 50  0001 C CNN
F 1 "+5V" H 1215 5673 50  0000 C CNN
F 2 "" H 1200 5500 50  0001 C CNN
F 3 "" H 1200 5500 50  0001 C CNN
	1    1200 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5F794B07
P 800 6450
F 0 "#PWR0107" H 800 6200 50  0001 C CNN
F 1 "GND" H 805 6277 50  0000 C CNN
F 2 "" H 800 6450 50  0001 C CNN
F 3 "" H 800 6450 50  0001 C CNN
	1    800  6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  6450 800  6300
Wire Wire Line
	700  6250 700  6300
Wire Wire Line
	700  6300 800  6300
Connection ~ 800  6300
Wire Wire Line
	800  6300 800  6250
$Comp
L power:GND #PWR0108
U 1 1 5F796869
P 1500 7450
F 0 "#PWR0108" H 1500 7200 50  0001 C CNN
F 1 "GND" H 1505 7277 50  0000 C CNN
F 2 "" H 1500 7450 50  0001 C CNN
F 3 "" H 1500 7450 50  0001 C CNN
	1    1500 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 7250 2300 7350
Wire Wire Line
	2300 7350 2500 7350
Connection ~ 2500 7350
Wire Wire Line
	2500 7350 2500 7250
Wire Wire Line
	2600 7250 2600 7350
Wire Wire Line
	2600 7350 2500 7350
Wire Wire Line
	2700 7250 2700 7350
Wire Wire Line
	2700 7350 2600 7350
Connection ~ 2600 7350
Wire Wire Line
	1700 6950 1500 6950
Wire Wire Line
	1500 6950 1500 7350
Wire Wire Line
	3300 5550 3450 5550
Wire Wire Line
	3300 5650 3450 5650
Text GLabel 3450 5550 2    50   Output ~ 0
TXD
Text GLabel 3450 5650 2    50   Input ~ 0
RXD
Wire Wire Line
	3150 3200 3250 3200
Wire Wire Line
	3150 3300 3250 3300
Text GLabel 3250 3200 2    50   Input ~ 0
TXD
Text GLabel 3250 3300 2    50   Output ~ 0
RXD
$Comp
L Device:R R4
U 1 1 5F7A6CD7
P 3600 2750
F 0 "R4" H 3530 2704 50  0000 R CNN
F 1 "10k" H 3530 2795 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3530 2750 50  0001 C CNN
F 3 "~" H 3600 2750 50  0001 C CNN
	1    3600 2750
	-1   0    0    1   
$EndComp
Wire Wire Line
	3150 3000 3600 3000
Wire Wire Line
	3600 3000 3600 2900
$Comp
L power:+5V #PWR0109
U 1 1 5F7A9850
P 3600 2550
F 0 "#PWR0109" H 3600 2400 50  0001 C CNN
F 1 "+5V" H 3615 2723 50  0000 C CNN
F 2 "" H 3600 2550 50  0001 C CNN
F 3 "" H 3600 2550 50  0001 C CNN
	1    3600 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2550 3600 2600
Wire Wire Line
	3600 3000 3750 3000
Connection ~ 3600 3000
$Comp
L Device:C C5
U 1 1 5F7AE022
P 3900 3000
F 0 "C5" V 3648 3000 50  0000 C CNN
F 1 "100n" V 3739 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3938 2850 50  0001 C CNN
F 3 "~" H 3900 3000 50  0001 C CNN
	1    3900 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	4050 3000 4150 3000
Text GLabel 4150 3000 2    50   Input ~ 0
FT_RESET
Wire Wire Line
	3300 5950 3450 5950
Text GLabel 3450 5950 2    50   Output ~ 0
FT_RESET
$Comp
L Device:C C6
U 1 1 5F7B1CFD
P 1500 5550
F 0 "C6" V 1248 5550 50  0000 C CNN
F 1 "100n" V 1339 5550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1538 5400 50  0001 C CNN
F 3 "~" H 1500 5550 50  0001 C CNN
	1    1500 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 5550 1700 5550
$Comp
L power:GND #PWR0110
U 1 1 5F7B5673
P 1300 6250
F 0 "#PWR0110" H 1300 6000 50  0001 C CNN
F 1 "GND" H 1305 6077 50  0000 C CNN
F 2 "" H 1300 6250 50  0001 C CNN
F 3 "" H 1300 6250 50  0001 C CNN
	1    1300 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6250 1300 5550
Wire Wire Line
	1300 5550 1350 5550
Wire Wire Line
	2300 7350 1500 7350
Connection ~ 2300 7350
Wire Wire Line
	1500 7450 1500 7350
Connection ~ 1500 7350
$Comp
L Device:LED D5
U 1 1 5F7BE69B
P 3950 6650
F 0 "D5" H 3943 6487 50  0000 C CNN
F 1 "LED" H 3943 6396 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 3950 6650 50  0001 C CNN
F 3 "~" H 3950 6650 50  0001 C CNN
	1    3950 6650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 5F7BEDF1
P 3950 6550
F 0 "D4" H 3943 6767 50  0000 C CNN
F 1 "LED" H 3943 6676 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 3950 6550 50  0001 C CNN
F 3 "~" H 3950 6550 50  0001 C CNN
	1    3950 6550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 5F7BF5DB
P 4200 6350
F 0 "#PWR0111" H 4200 6200 50  0001 C CNN
F 1 "+5V" H 4215 6523 50  0000 C CNN
F 2 "" H 4200 6350 50  0001 C CNN
F 3 "" H 4200 6350 50  0001 C CNN
	1    4200 6350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5F7BFC57
P 3550 6550
F 0 "R5" V 3343 6550 50  0000 C CNN
F 1 "1k" V 3434 6550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3480 6550 50  0001 C CNN
F 3 "~" H 3550 6550 50  0001 C CNN
	1    3550 6550
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5F7BFF01
P 3550 6650
F 0 "R6" V 3665 6650 50  0000 C CNN
F 1 "1k" V 3756 6650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3480 6650 50  0001 C CNN
F 3 "~" H 3550 6650 50  0001 C CNN
	1    3550 6650
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 6650 3400 6650
Wire Wire Line
	3300 6550 3400 6550
$Comp
L power:GND #PWR0112
U 1 1 5F7D3B1E
P 2550 4500
F 0 "#PWR0112" H 2550 4250 50  0001 C CNN
F 1 "GND" H 2555 4327 50  0000 C CNN
F 2 "" H 2550 4500 50  0001 C CNN
F 3 "" H 2550 4500 50  0001 C CNN
	1    2550 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 4500 2550 4200
$Comp
L power:+5V #PWR0113
U 1 1 5F7D6565
P 2550 950
F 0 "#PWR0113" H 2550 800 50  0001 C CNN
F 1 "+5V" H 2565 1123 50  0000 C CNN
F 2 "" H 2550 950 50  0001 C CNN
F 3 "" H 2550 950 50  0001 C CNN
	1    2550 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 950  2550 1050
$Comp
L Device:Crystal Y1
U 1 1 5F7D8D68
P 3700 2150
F 0 "Y1" V 3654 2281 50  0000 L CNN
F 1 "20MHz" V 3745 2281 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_HC49-SD" H 3700 2150 50  0001 C CNN
F 3 "~" H 3700 2150 50  0001 C CNN
	1    3700 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 2200 3550 2200
Wire Wire Line
	3550 2200 3550 2300
Wire Wire Line
	3550 2300 3700 2300
Wire Wire Line
	3150 2100 3550 2100
Wire Wire Line
	3550 2100 3550 2000
Wire Wire Line
	3550 2000 3700 2000
$Comp
L Device:C C3
U 1 1 5F7DE380
P 4150 2000
F 0 "C3" V 3898 2000 50  0000 C CNN
F 1 "22p" V 3989 2000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4188 1850 50  0001 C CNN
F 3 "~" H 4150 2000 50  0001 C CNN
	1    4150 2000
	0    1    1    0   
$EndComp
$Comp
L Device:C C4
U 1 1 5F7DE864
P 4150 2300
F 0 "C4" V 4310 2300 50  0000 C CNN
F 1 "22p" V 4401 2300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4188 2150 50  0001 C CNN
F 3 "~" H 4150 2300 50  0001 C CNN
	1    4150 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 2300 3700 2300
Connection ~ 3700 2300
Wire Wire Line
	3700 2000 4000 2000
Connection ~ 3700 2000
Wire Wire Line
	4300 2000 4500 2000
Wire Wire Line
	4500 2000 4500 2300
Wire Wire Line
	4300 2300 4500 2300
Connection ~ 4500 2300
Wire Wire Line
	4500 2300 4500 2400
$Comp
L power:GND #PWR0114
U 1 1 5F7E9575
P 4500 2400
F 0 "#PWR0114" H 4500 2150 50  0001 C CNN
F 1 "GND" H 4505 2227 50  0000 C CNN
F 2 "" H 4500 2400 50  0001 C CNN
F 3 "" H 4500 2400 50  0001 C CNN
	1    4500 2400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5F808FB7
P 10650 5850
F 0 "H1" H 10750 5899 50  0000 L CNN
F 1 "MountingHole_Pad" H 10750 5808 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad_Via" H 10650 5850 50  0001 C CNN
F 3 "~" H 10650 5850 50  0001 C CNN
	1    10650 5850
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4148 D3
U 1 1 5F81157A
P 7200 1600
F 0 "D3" H 7200 1383 50  0000 C CNN
F 1 "1N4148" H 7200 1474 50  0000 C CNN
F 2 "Diode_SMD:D_MiniMELF" H 7200 1425 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 7200 1600 50  0001 C CNN
	1    7200 1600
	-1   0    0    1   
$EndComp
Wire Wire Line
	7050 1600 7000 1600
$Comp
L Device:R R2
U 1 1 5F816094
P 8000 2100
F 0 "R2" V 7793 2100 50  0000 C CNN
F 1 "1k" V 7884 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7930 2100 50  0001 C CNN
F 3 "~" H 8000 2100 50  0001 C CNN
	1    8000 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 1600 8000 1600
Wire Wire Line
	8000 1950 8000 1600
Connection ~ 8000 1600
Wire Wire Line
	8000 1600 8300 1600
$Comp
L power:GND #PWR0115
U 1 1 5F81F11A
P 8000 2400
F 0 "#PWR0115" H 8000 2150 50  0001 C CNN
F 1 "GND" H 8005 2227 50  0000 C CNN
F 2 "" H 8000 2400 50  0001 C CNN
F 3 "" H 8000 2400 50  0001 C CNN
	1    8000 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2400 8000 2250
Wire Wire Line
	8000 1600 8000 1400
$Comp
L Device:LED D6
U 1 1 5F828FB0
P 1700 4800
F 0 "D6" H 1693 5017 50  0000 C CNN
F 1 "LED" H 1693 4926 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 1700 4800 50  0001 C CNN
F 3 "~" H 1700 4800 50  0001 C CNN
	1    1700 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5F829392
P 1300 4800
F 0 "R8" V 1093 4800 50  0000 C CNN
F 1 "1k" V 1184 4800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1230 4800 50  0001 C CNN
F 3 "~" H 1300 4800 50  0001 C CNN
	1    1300 4800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F829BD3
P 1050 4900
F 0 "#PWR04" H 1050 4650 50  0001 C CNN
F 1 "GND" H 1055 4727 50  0000 C CNN
F 2 "" H 1050 4900 50  0001 C CNN
F 3 "" H 1050 4900 50  0001 C CNN
	1    1050 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 4900 1050 4800
Wire Wire Line
	1050 4800 1150 4800
Wire Wire Line
	1450 4800 1550 4800
$Comp
L power:+5V #PWR03
U 1 1 5F83018C
P 1900 4700
F 0 "#PWR03" H 1900 4550 50  0001 C CNN
F 1 "+5V" H 1915 4873 50  0000 C CNN
F 2 "" H 1900 4700 50  0001 C CNN
F 3 "" H 1900 4700 50  0001 C CNN
	1    1900 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4700 1900 4800
Wire Wire Line
	1900 4800 1850 4800
$Comp
L Device:C C8
U 1 1 5F78CCE8
P 1500 6650
F 0 "C8" H 1615 6604 50  0000 L CNN
F 1 "100n" H 1615 6695 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1538 6500 50  0001 C CNN
F 3 "~" H 1500 6650 50  0001 C CNN
	1    1500 6650
	-1   0    0    1   
$EndComp
Wire Wire Line
	1500 6500 1500 6250
Connection ~ 1500 6250
Wire Wire Line
	1500 6950 1500 6800
Connection ~ 1500 6950
$Comp
L Device:C C7
U 1 1 5F79D294
P 2200 1050
F 0 "C7" V 1948 1050 50  0000 C CNN
F 1 "100n" V 2039 1050 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2238 900 50  0001 C CNN
F 3 "~" H 2200 1050 50  0001 C CNN
	1    2200 1050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5F79ED98
P 1850 1150
F 0 "#PWR02" H 1850 900 50  0001 C CNN
F 1 "GND" H 1855 977 50  0000 C CNN
F 2 "" H 1850 1150 50  0001 C CNN
F 3 "" H 1850 1150 50  0001 C CNN
	1    1850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1150 1850 1050
Wire Wire Line
	1850 1050 2050 1050
Wire Wire Line
	2350 1050 2550 1050
Connection ~ 2550 1050
Wire Wire Line
	2550 1050 2550 1200
$Comp
L Connector:AudioJack2_SwitchT J4
U 1 1 5F7A69DD
P 8000 700
F 0 "J4" V 7986 888 50  0000 L CNN
F 1 "AudioJack2_SwitchT" V 8077 888 50  0000 L CNN
F 2 "Custom:3.5mm_narrow" H 8000 700 50  0001 C CNN
F 3 "~" H 8000 700 50  0001 C CNN
	1    8000 700 
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5F7A7920
P 8100 1100
F 0 "#PWR01" H 8100 850 50  0001 C CNN
F 1 "GND" H 8105 927 50  0000 C CNN
F 2 "" H 8100 1100 50  0001 C CNN
F 3 "" H 8100 1100 50  0001 C CNN
	1    8100 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1100 8100 1000
$Comp
L Device:R R7
U 1 1 5F7AB75F
P 8000 1250
F 0 "R7" H 8069 1204 50  0000 L CNN
F 1 "100" H 8069 1295 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7930 1250 50  0001 C CNN
F 3 "~" H 8000 1250 50  0001 C CNN
	1    8000 1250
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 1100 8000 900 
$Comp
L Connector:AudioJack2_SwitchT J3
U 1 1 5F7B4182
P 7550 700
F 0 "J3" V 7233 679 50  0000 C CNN
F 1 "AudioJack2_SwitchT" V 7324 679 50  0000 C CNN
F 2 "Custom:3.5mm_narrow" H 7550 700 50  0001 C CNN
F 3 "~" H 7550 700 50  0001 C CNN
	1    7550 700 
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 900  7650 1000
Wire Wire Line
	7650 1000 8100 1000
Connection ~ 8100 1000
Wire Wire Line
	8100 1000 8100 900 
Wire Wire Line
	7550 900  7550 1850
Wire Wire Line
	7000 1850 7550 1850
Connection ~ 7550 1850
Wire Wire Line
	7550 1850 8300 1850
$Comp
L Connector:AVR-ISP-6 J5
U 1 1 5F7ED770
P 6850 5100
F 0 "J5" H 6521 5196 50  0000 R CNN
F 1 "AVR-ISP-6" H 6521 5105 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" V 6600 5150 50  0001 C CNN
F 3 " ~" H 5575 4550 50  0001 C CNN
	1    6850 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5F7EF9FA
P 6750 5650
F 0 "#PWR06" H 6750 5400 50  0001 C CNN
F 1 "GND" H 6755 5477 50  0000 C CNN
F 2 "" H 6750 5650 50  0001 C CNN
F 3 "" H 6750 5650 50  0001 C CNN
	1    6750 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 5650 6750 5500
Wire Wire Line
	6750 4450 6750 4600
$Comp
L power:+5V #PWR05
U 1 1 5F7F9469
P 6750 4450
F 0 "#PWR05" H 6750 4300 50  0001 C CNN
F 1 "+5V" H 6765 4623 50  0000 C CNN
F 2 "" H 6750 4450 50  0001 C CNN
F 3 "" H 6750 4450 50  0001 C CNN
	1    6750 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 3000 3600 3150
Wire Wire Line
	3600 3150 4150 3150
Text GLabel 4150 3150 2    50   Input ~ 0
RESET
Wire Wire Line
	7250 5200 7500 5200
Text GLabel 7500 5200 2    50   Output ~ 0
RESET
Wire Wire Line
	7250 5100 7500 5100
Wire Wire Line
	7250 5000 7500 5000
Wire Wire Line
	7250 4900 7500 4900
Text GLabel 7500 4900 2    50   Input ~ 0
MISO
Text GLabel 7500 5000 2    50   Output ~ 0
MOSI
Text GLabel 7500 5100 2    50   Output ~ 0
SCK
Wire Wire Line
	3150 1800 3300 1800
Wire Wire Line
	3150 1900 3300 1900
Text GLabel 3300 1900 2    50   Output ~ 0
MISO
Text GLabel 3300 1800 2    50   Input ~ 0
MOSI
Text GLabel 3300 2000 2    50   Input ~ 0
SCK
Wire Wire Line
	3150 2000 3300 2000
Wire Wire Line
	3150 1500 3300 1500
Text GLabel 3300 1500 2    50   Output ~ 0
XIN
Wire Wire Line
	3700 6550 3800 6550
Wire Wire Line
	3700 6650 3800 6650
Wire Wire Line
	4100 6650 4200 6650
Wire Wire Line
	4200 6650 4200 6550
Wire Wire Line
	4100 6550 4200 6550
Connection ~ 4200 6550
Wire Wire Line
	4200 6550 4200 6350
$EndSCHEMATC
