Certainly! Here is a refined and detailed README for your Theme Park Simulation project:

---

# Theme Park Simulation

## Introduction

Welcome to the Theme Park Simulation! This project is a virtual theme park experience implemented in C. Visitors can enjoy various rides, participate in trivia challenges, visit a haunted house (for those 18 and older), and purchase items from the food court. The simulation tracks the park's total revenue and provides detailed receipts for each visitor.

## Features

1. **Intro Page**
   - The simulation begins with an introduction page that sets the theme and mood for the virtual theme park experience.

2. **Rides**
   - Visitors can queue up for various rides.
   - If visitors don't get a chance to ride in the first round, they will be asked again once the rides are empty, ensuring everyone gets a turn.

3. **Haunted House**
   - Only visitors aged 18 and above are allowed entry.
   - Visitors under 18 are not permitted in the haunted house.
   - Visitors participate in a trivia challenge.
   - Correct answers allow visitors to surpass the haunted house doors and experience a creepy event.
   - There are four doors to pass through.
   - Visitors who answer all questions correctly win a prize.

4. **Food Court**
   - The menu is retrieved from a CSV file.
   - Visitors can purchase as many items as they want in whatever quantities they desire.
   - At the end of their purchase, a receipt is generated detailing their items and total cost.
   - The total revenue of the theme park is stored in a revenue CSV file.

## How to Compile and Run the Simulation

### Setup
1. Ensure you have a C compiler (such as `gcc`) installed on your machine.
2. Place the menu CSV file (`menu.csv`) and the revenue CSV file (`revenue.csv`) in the project directory.

### Compiling the Simulation
1. Open a terminal or command prompt.
2. Navigate to the project directory.
3. Compile the code using the provided Makefile:
   ```bash
   make
   ```

### Running the Simulation
1. Run the compiled executable:
   ```bash
   ./theme_park_simulation
   ```
2. Follow the on-screen prompts to navigate through the different sections of the theme park.
3. Input your choices when prompted to experience the various rides, participate in the trivia challenge, visit the haunted house, and purchase items from the food court.

## CSV File Formats

### Menu CSV (`menu.csv`)
This file should contain the menu items available at the food court. Each row represents an item with the following columns:
- `Item Name`: The name of the food item.
- `Price`: The price of the item.

Example:
```csv
Item Name,Price
Burger,5.99
Pizza,7.49
Soda,1.99
```

### Revenue CSV (`revenue.csv`)
This file keeps track of the total revenue generated by the theme park. It contains the following columns:
- `Category`: The theme park attraction (with "Total" as the final category).
- `Revenue`: The revenue for that category.

Example:
```csv
Category,Revenue
Rides,1500.00
Haunted House,600.00
Food Court,400.00
Total,2500.00
```

## Notes

- Make sure the CSV files are correctly formatted and placed in the appropriate directory.
- The simulation is designed to be interactive and user-friendly. Follow the prompts carefully to fully enjoy the virtual theme park experience.

## Future Enhancements

- Add more rides and attractions.
- Implement a more complex queuing system for rides.
- Add more detailed reports and analytics for revenue and visitor statistics.

## Contributing

If you would like to contribute to the project, please fork the repository and submit a pull request with your changes. Ensure your code is well-documented and adheres to the project's coding standards.

### Contribution Steps
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes to your branch.
4. Submit a pull request with a detailed description of your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

Enjoy your visit to the virtual theme park! If you have any questions or feedback, please feel free to reach out.

---

### Additional Suggestions

1. **Screenshots and Visuals**
   - Add screenshots or visuals of the simulation running to make the README more engaging.

2. **Detailed Contribution Guidelines**
   - Provide a `CONTRIBUTING.md` file with detailed steps on how to contribute, including setting up the development environment, running tests, and submitting pull requests.

3. **Usage Examples**
   - Include example outputs or sample interactions to give users a clear idea of what to expect.

4. **Testing**
   - Provide information on how to run tests if applicable.

5. **FAQs**
   - Include a Frequently Asked Questions section to address common issues or questions users might have.

By incorporating these suggestions, you can make your README more informative and user-friendly, helping potential users and contributors understand and engage with your project better.
