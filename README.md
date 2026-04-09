# 🔧 Car Workshop Management System

![C++](https://img.shields.io/badge/c++-11%2F14-blue.svg?style=flat-square&logo=c%2B%2B)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg?style=flat-square)

A robust, terminal-based management system for car repair shops, written in modern C++. It streamlines vehicle intake, automated diagnostics, dynamic cost calculation, and daily revenue reporting. 

<div align="center">
  <img src="demo.gif" alt="Workshop System Demo CLI" width="700"/>
  <p><i>Real-time terminal execution of the Workshop Management System.</i></p>
</div>

## 🎯 Value Proposition

This project was developed to showcase practical, real-world applications of **modern C++**. Rather than just basic logic, it focuses on robust software architecture, proper memory lifecycle management, and scalable OOP design patterns.

## 🚀 Key Technical Features

* **Advanced OOP Architecture:** Heavily relies on polymorphism, abstraction (`IPojazd`), and inheritance to handle different vehicle behaviors (Cars, Motorcycles, Trucks) cleanly without massive `if/else` chains.
* **Smart Memory Management:** Complete elimination of raw pointers. Uses `std::unique_ptr` and standard containers (`std::vector`, `std::map`) to guarantee leak-free memory handling during active and archived work orders.
* **Custom Exception Hierarchy:** Implements robust error handling via a tailored exception tree (extending `std::runtime_error`) for file I/O operations and input validation.
* **Dynamic Pricing Engine:** Reads and applies real-time cost and time multipliers from external config files (`marki.txt`), simulating a scalable database approach.
* **Automated Data Persistence:** Features a daily reporting module that generates formatted `.txt` summaries including revenue, processed vehicles, and workshop efficiency metrics.

##
