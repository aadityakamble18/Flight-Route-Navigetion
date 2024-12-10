# CSL2020 | Data Structure and Algorithms | Jan 2024

**Instructor:** Suchetana Chakraborty

## Group Project:
- **Topic:** Navigating Airports Using Graph Algorithms
- **Mentor TAs:**
  - Jainan Nareshkumar Tandel (M23CSA010)
  - Dhruv (B20EE016)

## Team Members:
1. Vishwjeetsinh Jadeja (B22MT023)
2. Aaditya Kamble (B22MT024) - [b22mt024@iitj.ac.in](mailto:b22mt024@iitj.ac.in)
3. Sahil (B22MT038)
4. Ghanshyam Suthar (B22PH009)

**GitHub Repo:** [ideathon-code-submission-b22mt024](https://github.com/DSA-IITJ-2024/ideathon-code-submission-b22mt024)

## Contents:
- **CSV Files:**
  1. `airports_final.csv`: Contains all the airport information used as vertices.
  2. `routes_final.csv`: Contains all the routes data used for making edges.
- **C Code Files:**
  1. `dijkstra.c`
  2. `Bellman.c`
  3. `floyd.c`
- **Other Files:**
  1. `visualization.svg`: Shows the visualization of the graphs.
  2. `Distance_matrix.txt`: Contains the output matrix given by the Floyd algorithm.
  3. `timecomplexity.xlsx`: Contains all the results and graphs.
  4. `DSAIdeathon.pptx`: Presentation.
  5. `DSAIdeathon.pdf`: Presentation.

**PPT Link:** [View Presentation](https://www.canva.com/design/DAGD0ScPLPQ/Yi3yC_BpVxUc4rswWLn8ow/edit)

## How to Use:
1. Make sure that the two CSV files and the three code files are placed in the same folder.
2. Open the command prompt in the folder where all the files are placed.
3. Use the following commands to compile:
   - **Dijkstra:**
     ```sh
     gcc -w dijkstra.c -o dijkstra
     ```
   - **Bellman-Ford:**
     ```sh
     gcc -w Bellman.c -o bellman
     ```
   - **Floyd:**
     ```sh
     gcc -w floyd.c -o floyd
     ```
   Ensure to include the `-w` flag while compiling to avoid any warnings.
4. To execute the programs, use the following commands:
   - **Dijkstra:**
     ```sh
     ./dijkstra
     ```
   - **Bellman-Ford:**
     ```sh
     ./bellman
     ```
   - **Floyd:**
     ```sh
     ./floyd
     ```
5. If you are using VS Code, you can run the following commands in the terminal:
   - **dijkstra.c:**
     ```sh
     ./dijkstra
     ```
   - **Bellman.c:**
     ```sh
     ./bellman
     ```
   - **floyd.c:**
     ```sh
     ./floyd
     ```
6. After executing, the program will ask for input and output IATA codes. You can refer to the CSV files for airport information. The routes CSV file contains information about the source and destination airports.
7. Use the `visualization.svg` file to verify the output.

**Note:** Ensure that the `-w` flag is included while compiling to avoid warnings.
