import os
import csv
import math
import random


def validate_probability(probability: float) -> bool:
    return 0 <= probability <= 1


class FunctionMaximum:
    population = None
    population_probabilities = None
    population_cumulated_probabilities = None

    population_after_selection = None
    population_selection_probabilities = None
    population_selection_indices = None

    population_after_crossover = None
    population_crossover_probabilities = None
    population_crossover_results = None

    population_after_mutation = None
    population_mutation_probabilities = None
    population_mutation_results = None

    performance_evolution = None

    def __init__(self, population_size: int, step_count: int, precision: int, bounds: tuple[float, float],
                 coefficients: tuple[float, float, float], crossover_probability: float, mutation_probability: float):
        if bounds[0] > bounds[1]:
            raise Exception("Lower bound should be smaller than upper bound!")

        if not validate_probability(crossover_probability):
            raise Exception("Crossover probability should be between 0 and 1!")

        if not validate_probability(mutation_probability):
            raise Exception("Mutation probability should be between 0 and 1!")

        self.population_size = population_size
        self.step_count = step_count
        self.precision = precision

        self.bounds = bounds
        self.coefficients = coefficients

        self.crossover_probability = crossover_probability
        self.mutation_probability = mutation_probability

        self.individual_size = math.ceil(math.log2((self.bounds[1] - self.bounds[0]) * 10 ** self.precision))

    def get_individual_binary_value(self, individual: int) -> str:
        representation = ''
        for power in reversed(range(0, self.individual_size)):
            if individual & (2 ** power) != 0:
                representation += '1'
            else:
                representation += '0'
        return representation

    def get_individual_bounds_value(self, individual: int) -> float:
        return self.bounds[0] + ((self.bounds[1] - self.bounds[0]) * individual) / (2 ** self.individual_size - 1)

    def get_individual_performance(self, individual: int) -> float:
        bounds_value = self.get_individual_bounds_value(individual)
        return self.coefficients[0] * (bounds_value ** 2) + self.coefficients[1] * bounds_value + self.coefficients[2]

    def get_population_performance(self) -> float:
        performance = 0
        for individual in self.population:
            performance += self.get_individual_performance(individual)
        return performance

    def generate_new_population(self):
        self.population = []
        for index in range(self.population_size):
            individual = random.randint(0, (2 ** self.individual_size) - 1)
            self.population.append(individual)

    def generate_population_probabilities(self):
        self.population_probabilities = []
        self.population_cumulated_probabilities = []

        current_cumulated_probability = 0
        population_performance = self.get_population_performance()
        maximum_performance = 0
        mean_performance = 0

        for individual in self.population:
            individual_performance = self.get_individual_performance(individual)
            maximum_performance = max(maximum_performance, individual_performance)
            mean_performance += individual_performance
            individual_probability = individual_performance / population_performance
            self.population_probabilities.append(individual_probability)
            current_cumulated_probability += individual_probability
            self.population_cumulated_probabilities.append(current_cumulated_probability)

        mean_performance /= self.population_size
        self.performance_evolution.append((maximum_performance, mean_performance))

    def run_selection(self):
        # Select best performer
        best_performance = 0
        best_performer_index = 0
        for individual_index in range(self.population_size):
            individual = self.population[individual_index]
            performance = self.get_individual_performance(individual)
            if performance > best_performance:
                best_performance = performance
                best_performer_index = individual_index

        self.population_after_selection = [self.population[best_performer_index]]
        self.population_selection_probabilities = [0]
        self.population_selection_indices = [best_performer_index]

        for selection_step in range(self.population_size - 1):
            selection_probability = random.random()
            self.population_selection_probabilities.append(selection_probability)

            left = 0
            right = self.population_size - 1
            selected_individual = 0
            while left < right:
                middle = (left + right) // 2
                cumulated_probability = self.population_cumulated_probabilities[middle]
                if selection_probability >= cumulated_probability:
                    selected_individual = middle
                    left = middle + 1
                else:
                    right = middle - 1

            self.population_selection_indices.append(selected_individual)
            self.population_after_selection.append(self.population[selected_individual])

    def get_crossover_result(self, first_individual: int, second_individual: int) -> tuple[int, int, int]:
        break_point = random.randint(1, self.individual_size - 1)
        bitmask = (2 ** break_point) - 1

        first_individual_crossed_over = first_individual - (first_individual & bitmask) + (second_individual & bitmask)
        second_individual_crossed_over = second_individual - (second_individual & bitmask) + (
                    first_individual & bitmask)

        return first_individual_crossed_over, second_individual_crossed_over, break_point

    def run_crossover(self):
        self.population_after_crossover = []
        self.population_crossover_probabilities = []
        self.population_crossover_results = []

        individuals_for_crossover = []
        for individual_index in range(self.population_size):
            crossover_probability = random.random()
            individual = self.population_after_selection[individual_index]

            self.population_crossover_probabilities.append(crossover_probability)
            self.population_after_crossover.append(individual)

            if crossover_probability < self.crossover_probability:
                individuals_for_crossover.append((individual, individual_index))

        for individual_index in range(1, len(individuals_for_crossover), 2):
            first_individual = individuals_for_crossover[individual_index - 1]
            second_individual = individuals_for_crossover[individual_index]

            crossover_data = self.get_crossover_result(first_individual[0], second_individual[0])

            self.population_after_crossover[first_individual[1]] = crossover_data[0]
            self.population_after_crossover[second_individual[1]] = crossover_data[1]

            self.population_crossover_results.append((first_individual[0], second_individual[0]) + crossover_data)

    def get_mutation_result(self, individual: int) -> tuple[int, int]:
        position = random.randint(0, self.individual_size - 1)
        mutated_individual = individual ^ (2 ** position)

        return mutated_individual, position

    def run_mutation(self):
        self.population_after_mutation = []
        self.population_mutation_probabilities = []
        self.population_mutation_results = []

        for individual_index in range(self.population_size):
            mutation_probability = random.random()
            individual = self.population_after_crossover[individual_index]

            self.population_mutation_probabilities.append(mutation_probability)

            if mutation_probability < self.mutation_probability:
                mutation_data = self.get_mutation_result(individual)
                self.population_mutation_results.append((individual,) + mutation_data)
                individual = mutation_data[0]

            self.population_after_mutation.append(individual)

    def get_individual_data_for_report(self, individual: int):
        return [self.get_individual_binary_value(individual), self.get_individual_bounds_value(individual),
                self.get_individual_performance(individual)]

    def generate_population_report(self, directory_name: str, file_name: str):
        path = os.path.normpath(directory_name + '/' + file_name)
        with open(path, 'w') as population_file:
            population_writer = csv.writer(population_file, delimiter=",")

            population_writer.writerow(["Initial population"])
            population_writer.writerow(
                ['Individual index', 'Integer representation', 'Binary representation', 'Bounds value',
                 'Function value', 'Selection probability'])
            for individual_index in range(self.population_size):
                individual = self.population[individual_index]
                individual_data = self.get_individual_data_for_report(individual)
                selection_probability = self.population_probabilities[individual_index]
                population_writer.writerow(
                    [individual_index + 1, individual] + individual_data + [selection_probability])

            population_writer.writerow(["Population after selection"])
            population_writer.writerow(
                ['Individual index', 'Integer representation', 'Binary representation', 'Bounds value',
                 'Function value', 'Generated probability', 'Initial index'])
            for individual_index in range(self.population_size):
                individual = self.population_after_selection[individual_index]
                individual_data = self.get_individual_data_for_report(individual)
                individual_probability = self.population_selection_probabilities[individual_index]
                initial_index = self.population_selection_indices[individual_index]
                population_writer.writerow(
                    [individual_index + 1, individual] + individual_data + [individual_probability, initial_index])

            population_writer.writerow(["Crossover participants"])
            population_writer.writerow(["First participant", "Second participant", "First participant crossed over", "Second participant crossed over", "Crossover position"])
            for entry in self.population_crossover_results:
                processed_entry = []
                for entry_index in range(4):
                    processed_entry.append(self.get_individual_binary_value(entry[entry_index]))
                processed_entry.append(entry[-1])
                population_writer.writerow(processed_entry)

            population_writer.writerow([f"Population after crossover with probability {self.crossover_probability}"])
            population_writer.writerow(
                ['Individual index', 'Integer representation', 'Binary representation', 'Bounds value',
                 'Function value', 'Generated probability'])
            for individual_index in range(self.population_size):
                individual = self.population_after_selection[individual_index]
                individual_data = self.get_individual_data_for_report(individual)
                individual_probability = self.population_selection_probabilities[individual_index]
                if individual_probability < self.crossover_probability:
                    individual_probability = f"{individual_probability}<{self.crossover_probability}"
                population_writer.writerow(
                    [individual_index + 1, individual] + individual_data + [individual_probability])

            population_writer.writerow(["Mutation participants"])
            population_writer.writerow(["Individual", "Mutated individual", "Mutation position"])
            for entry in self.population_mutation_results:
                processed_entry = []
                for entry_index in range(2):
                    processed_entry.append(self.get_individual_binary_value(entry[entry_index]))
                processed_entry.append(entry[-1])
                population_writer.writerow(list(processed_entry))

            population_writer.writerow([f"Population after mutation with probability {self.mutation_probability}"])
            population_writer.writerow(
                ['Individual index', 'Integer representation', 'Binary representation', 'Bounds value',
                 'Function value', 'Generated probability'])
            for individual_index in range(self.population_size):
                individual = self.population_after_mutation[individual_index]
                individual_data = self.get_individual_data_for_report(individual)
                individual_probability = self.population_mutation_probabilities[individual_index]
                if individual_probability < self.mutation_probability:
                    individual_probability = f"{individual_probability}<{self.crossover_probability}"
                population_writer.writerow(
                    [individual_index + 1, individual] + individual_data + [individual_probability])

    def generate_evolution_report(self, directory_name: str, file_name: str):
        path = os.path.normpath(directory_name + '/' + file_name)
        with open(path, 'w') as evolution_file:
            evolution_writer = csv.writer(evolution_file, delimiter=",")

            evolution_writer.writerow(['Generation index', 'Maximum fitness', 'Mean fitness'])
            for generation_index in range(self.step_count):
                generation_data = self.performance_evolution[generation_index]
                evolution_writer.writerow([generation_index + 1, generation_data[0], generation_data[1]])

    def run_simulation(self, generate_reports):
        directory_name = "src/teme/genetics/data"

        test.generate_new_population()
        self.performance_evolution = []
        for step_index in range(self.step_count):
            test.generate_population_probabilities()
            test.run_selection()
            test.run_crossover()
            test.run_mutation()

            if generate_reports:
                self.generate_population_report(directory_name, f"population-{step_index + 1}.csv")
            self.population = self.population_after_mutation

        if generate_reports:
            self.generate_evolution_report(directory_name, "population-evolution.csv")


test = FunctionMaximum(20, 50, 6, (-1, 2), (-1, 1, 2), 0.25, 0.01)
test.run_simulation(True)
