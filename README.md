# 🎢 Theme Park Simulation  

Welcome to the **Theme Park Simulation**! This project is a **virtual theme park experience** implemented in **C**, featuring rides, trivia challenges, a haunted house (18+), and a food court with a **real-time revenue tracking system**.  

## ✨Features✨

### 🏁 **Intro Page**  
- The simulation starts with a **thematic introduction page** setting the mood for the virtual experience.  

### 🎡 **Rides**  
- Visitors **queue up** for different rides.  
- If they don't get a turn in the first round, they are asked again when slots open, ensuring everyone gets a chance.

### 👻 **Haunted House** (18+)  
- Restricted to **visitors aged 18+**.  
- Includes a **trivia challenge** to progress through four eerie doors.  
- **🏆 Prize for winners** who answer all trivia questions correctly.  

### 🍔 **Food Court**  
- **Menu loaded from a CSV file** for easy customization.  
- Visitors can **buy multiple items** in any quantity.  
- Generates a **detailed receipt** after every purchase.  
- **Revenue updates in real-time** and are stored in a CSV file.  

## 🛠️ Tech Stack  

- **Programming Language:** C  
- **Data Handling:** CSV file management  
- **Data Structures:**  
  - **Queues** (for ride management)  
  - **Decision-based logic** (for trivia and haunted house)  
  - **File I/O operations** (for menu and revenue tracking)  

## 🚀 Installation  

### **Prerequisites**  
- A **C compiler** (e.g., `gcc`).  
- Ensure **menu.csv** and **revenue.csv** are present in the project directory.  

### **Compiling the Simulation**  
1. Open a terminal and navigate to the project folder.  
2. Compile using the provided **Makefile**:  
   ```bash
   make
   ```

### **Running the Simulation**  
1. Execute the compiled file:  
   ```bash
   ./theme_park_simulation
   ```
2. Follow the **on-screen prompts** to explore the theme park.  

## 📜 CSV File Formats  

### **Menu CSV (`menu.csv`)**  
Stores food court items with prices.  

```csv
Item Name,Price
Burger,5.99
Pizza,7.49
Soda,1.99
```

### **Revenue CSV (`revenue.csv`)**  
Tracks revenue from different park sections.  

```csv
Category,Revenue
Rides,1500.00
Haunted House,600.00
Food Court,400.00
Total,2500.00
```

## 🔮 Future Enhancements  
- Add **more rides and attractions**.  
- Implement an **advanced queuing system** for rides.  
- Introduce **detailed revenue analytics and reporting**.  

## 🤝 Contributions  

Contributions are welcome! If you'd like to help, fork the repo, create a new branch, and submit a pull request.

### 🎗 Special Thanks  
💡 **Credits to [completelyblank](https://github.com/completelyblank)** for his valuable contribution to the **intro pages** of this project.  
