import random


class Station:
    """Contains the name and delay probability of a station
    """
    def __init__(self, name, delay_prob):
        self.name = name.upper()
        self.delay_prob = delay_prob


    @staticmethod
    def get_station_obj(stations, name):
        """Gets a station object based on a given name

        Args:
            stations (list): list of station objects
            name (str): station name

        Returns:
            Station : station object
        """
        for station in stations:
            if station.name == name.upper():
                return station


    @staticmethod
    def get_stations():
        """Prompts the user for a stations file

        Returns:
            list: list of stations
        """
        stations = [] #list of stations
        while True:
            stations_file = ""
            try:
                stations_file = input("Enter the name of stations file: ")
                with open(stations_file) as f:
                    stations_f = f.readlines() #get a list of all the lines in the files
                    for line in stations_f:
                        cols = line.strip().split(",")
                        cols = [i for i in cols if i.strip()] #to remove empty strings
                        if len(cols) != 2:
                            continue
                        #Only process the line if it is not a blank space
                        if not line.isspace():
                            name = cols[0].strip()
                            try:
                                delay_prob = float(cols[1].strip())
                            except ValueError:
                                continue
                            #Creating the station object
                            station = Station(name, delay_prob)
                            #Adding the station to the list
                            stations.append(station)
                    if len(stations) == 0:
                        print("The file should have only two columns separated by a comma.")
                        print("where Column 1: str\tColumn2: float\n")
                        continue
                    return stations
            except IOError:
                print(f"File[{stations_file}] Not Found. Input a valid file.\n")
            except:
                pass

    def __str__(self):
        """Returns the string representation of the object

        Returns:
            str: name of this station object
        """
        return self.name


class Rail:
    """Contains the rail attributes such as source, target, line and direction
    """
    def __init__(self, source, target, line, direction):
        self.source = source
        self.target = target
        self.line = str(line).upper()
        self.direction = direction.upper()


    @staticmethod
    def get_connections(stations):
        """Creates a list of connections/Rail objects

        Args:
            stations (list): list of station objects

        Returns:
            list: list of connections/Rail objects
        """
        connections = [] #list of connections
        while True:
            connections_file = ""
            try:
                connections_file = input("Enter the name of connections file: ")
                with open(connections_file) as f:
                    connections_f = f.readlines() #get a list of all the lines in the file

                    for line in connections_f:
                        cols = line.strip().split(",")
                        cols = [i for i in cols if i.strip()] #to remove empty strings
                        if len(cols) != 4:
                            continue
                        #Only process the line if it is not a blank space
                        #and it contains 4 columns
                        if not line.isspace() and len(cols) >= 4:
                            source = Station.get_station_obj(stations, cols[0].strip())
                            target = Station.get_station_obj(stations, cols[1].strip())
                            line = cols[2].strip()
                            direction = cols[3].strip()

                            #Creating the connection object
                            connection = Rail(source, target, line, direction)
                            #Adding the connection to the list
                            connections.append(connection)
                    if len(connections) == 0:
                        print("The file should have only 4 columns separated by a comma.\n")
                        continue
                    return connections
            except IOError:
                print(f"File[{connections_file}] Not Found. Input a valid file.\n")
            except:
                pass

    def __str__(self):
        """Returns the string representation of a Rail object

        Returns:
            str: rail object string representation
        """
        return f"{self.source}-{self.target} ({self.line}, {self.direction})"


class Lines:
    """Contains all of the lines in the railway (ex: green, blue, etc.)
    """
    def __init__(self, connections):
        #Key: line name
        #Value: List containing the connected stations, ordered from north(index 0) to south(index -1)
        self.railways = {}
        self.create_lines(connections)


    def create_lines(self, connections):
        """Populates the self.railways with the lines

        Args:
            connections (list): list of Rail objects

        """
        for connection in connections:
            if connection.line not in self.railways.keys():
                self.railways[connection.line] = []

            #If else condition to make directions consistent with South direction
            #So source to target always direct to south direction
            if connection.direction == "S":
                source = connection.source
                target = connection.target
            else:
                source = connection.target
                target = connection.source

            line = self.railways[connection.line]
            #Adding the stations
            if len(line) == 0:
                line.append(source)
                line.append(target)
            else:
                index_of_source = line.index(source) #Get the source's index
                line.insert(index_of_source + 1, target) #Add the target after the source


    def get_line_names(self):
        """Gets a list of the railway names

        Returns:
            list: list of railway names /line names
        """
        return list(self.railways.keys())


    def __str__(self):
        """Gets the string representation of a Lines object

        Returns:
            str: Lines object string representation
        """
        railways_str = ""
        for line in self.railways:
            railways_str += (f"{line}: \n")
            for station in self.railways[line]:
                railways_str += (f"\t{station}")
            railways_str += "\n"
        return railways_str


class Train:
    """Contains the attributes for a train such as name, line name, direction and current direction
    """
    def __init__(self, name, line_name, direction, station):
        self.name = name
        self.line_name = line_name
        self.direction = direction
        self.station = station
        self.delayed = False


    def is_delayed(self):
        """Simulates the probability if the train is delayed or not based on its station's delay probability.
        When a train is delayed, this sets the delayed attribute to True, and returns True
        When a train is not delayed, this sets the delayed attribute to False, and returns False

        Returns:
            boolean: indicator if a train is delayed or not
        """
        if random.random() <= self.station.delay_prob:
            self.delayed = True
            return True
        else:
            self.delayed = False
            return False


    @staticmethod
    def get_trains(lines):
        """Creates the trains with random attribute values

        Args:
            lines (Lines): Lines object

        Returns:
            list: list of generated trains
        """
        trains = [] #List of trains
        while True:
            try:
                number_of_trains = int(input("Enter how many trains to simulate: "))

                for number in range(number_of_trains):
                    name = f"{number + 1}"
                    line_name = random.choice(lines.get_line_names()) #Randomy choose line name
                    direction = random.choice(["N", "S"]) #Randomly choose a direction
                    station = random.choice(lines.railways[line_name]) #Randomly choose a station base on the line
                    trains.append(Train(name, line_name, direction, station))
            except ValueError:
                print("Input a number only.")

            return trains


    def __str__(self):
        """Gets the string representation of a Train object

        Returns:
            str: Train object string representation
        """
        status = "(DELAY)" if self.delayed else ""
        direction = "North" if self.direction == "N" else "South"
        return f"Train {self.name} on {self.line_name} line is at station {self.station} heading in {direction} direction. {status}"


class RailwaySystem:
    def __init__(self, lines, trains, stations):
        self.lines = lines #lines object
        self.trains = trains #list of trains in the system
        self.stations = stations


    def update_train_direction(self, train):
        """Updates a single train object direction

        Args:
            train (Train): Train object

        Returns:
            Train: Train object
        """
        train_line = self.lines.railways[train.line_name]
        if train.station == train_line[0]:
            train.direction = "S"
        elif train.station == train_line[-1]:
            train.direction = "N"
        return train


    def update_all_train_directions(self):
        """Updates all train objects direction
        """
        for train in self.trains:
            self.update_train_direction(train)


    def simulate_one_step(self):
        """Simulates one train movement 
        """
        for train in self.trains:
            if train.is_delayed():
                pass
            else:
                #Train moves one station based on its direction
                #-1 going north (going left of the line/railway list)
                #+1 going north (going right of the line/railway list)
                direction = -1 if train.direction == "N" else 1
                train_line = self.lines.railways[train.line_name] #Line/railway list
                station_index = train_line.index(train.station) #Get the index of current station
                station_index += direction #Move to the next/previous station
                train.station = train_line[station_index] #Set new station
                self.update_train_direction(train) #Update train direction


    def get_train_info(self, train_number):
        """Gets the train info

        Args:
            train_number (int): train number in the list of trains

        Returns:
            str: train info
        """
        return f"\n{self.trains[train_number - 1]}"



    def create_graph(self):
        """Creates an undirected graph

        Returns:
            dict: undirected graph of all the stations
        """
        graph = {}

        for line_name in self.lines.railways:
            line = self.lines.railways[line_name]
            #Populating the graph dict with every available station in the line
            for station in line:
                if station.name not in graph:
                    graph[station.name] = []

                index_of_station = line.index(station)

                #Add the neigboring stations of the current station
                if index_of_station - 1 >= 0:
                    graph[station.name].append(line[index_of_station - 1].name)
                if index_of_station + 1 < len(line):
                    graph[station.name].append(line[index_of_station + 1].name)
        return graph


    def get_route_info(self, start, end, steps):
        """Gets route info

        Args:
            start (str): name of the starting station
            end (str): name of the goal station
            steps (str): max steps to be taken from start to the goal station

        Returns:
            str: route info
        """
        graph = self.create_graph()

        def shortest_stations_taken(graph, start, end, stations_taken = []):
            stations_taken = stations_taken + [start]
            if start == end:
                return stations_taken
            if start not in graph:
                return None
            else:
                shortest = None
                for node in graph[start]:
                    if node in stations_taken:
                        continue
                    newstations_taken = shortest_stations_taken(graph, node, end, stations_taken)
                    if newstations_taken:
                        if not shortest or len(newstations_taken) < len(shortest):
                            shortest = newstations_taken
                return shortest

        stations_taken = shortest_stations_taken(graph,start,end)

        if stations_taken == None:
            return f"\nStation {end} is not reachable from station {start}."
        else:
            if len(stations_taken) - 1 <= steps: #-1 to not count the start station
                return f"\nStation {end} is reachable from station {start} within {steps} timesteps."
            else:
                return f"\nStation {end} is not reachable from station {start} within {steps} timesteps."


    def run_simulation(self):
        """Simulates a train station at one timesteps
        """
        self.update_all_train_directions() #Initially Updating all train directions
        option = ""
        while option != "q":
            for train in self.trains:
                print(train)
            print("\nContinue simulation [1], train info [2], route_info[3], exit [q].")

            #Ensure a valid option input
            option = input("Select an option: ").strip()
            if(option not in ["1", "2", "3", "q"]):
                print("Option is invalid. Input a valid one.")
                continue

            if option == "1":
                self.simulate_one_step()
            elif option == "2":
                #Get a valid train_number input from user
                while True:
                    try:
                        train_number = int(input(f"Which train [1 - {len(self.trains)}]: "))
                        print(self.get_train_info(train_number))
                        break
                    except:
                        print("Input a valid train number.")
            elif option == "3":
                start = input("Select a start station: ").upper()
                end = input("Select an end station: ").upper()
                #Get a valid steps input from user
                while True:
                    try:
                        steps = int(input("Select timesteps: "))
                        print(self.get_route_info(start, end, steps))
                        break
                    except ValueError:
                        print("Input a valid timesteps input.")

        print("Thank you and goodbye!")


def main():
    stations = Station.get_stations() #List of Stations
    connections = Rail.get_connections(stations) #List of Connections
    lines = Lines(connections)
    print(lines)

    trains = Train.get_trains(lines) #List of Trains

    #Create the railway_system
    railway_system = RailwaySystem(lines, trains, stations)
    railway_system.run_simulation()


if __name__ == "__main__":
    main()
