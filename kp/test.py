import random

def generate_graph_tests(filename, n, m):
    """
    Генерация тестов для задачи графов с фиксированными запросами и запись в файл.

    :param filename: Имя файла для записи тестов.
    :param n: Количество вершин в графе.
    :param m: Количество рёбер в графе.
    """
    with open(filename, "w") as f:
        # Генерация числа вершин и рёбер
        f.write(f"{n} {m}\n")
        
        # Генерация координат вершин
        coordinates = []
        for i in range(n):
            x = random.randint(-10**3, 10**3)
            y = random.randint(-10**3, 10**3)
            coordinates.append((x, y))
            f.write(f"{x} {y}\n")
        
        # Генерация рёбер
        edges = set()
        adjacency_list = {i: set() for i in range(1, n + 1)}  # Для отслеживания смежности
        
        while len(edges) < m:
            u = random.randint(1, n)
            v = random.randint(1, n)
            if u != v and (u, v) not in edges and (v, u) not in edges:
                edges.add((u, v))
                adjacency_list[u].add(v)
                adjacency_list[v].add(u)  # Для неориентированного графа
        
        for u, v in edges:
            f.write(f"{u} {v}\n")
        
        # Генерация запросов
        q = 3  # Количество запросов
        f.write(f"{q}\n")

        # 1. Запрос на несоединённые вершины
        disconnected_pair = None
        for u in range(1, n + 1):
            for v in range(u + 1, n + 1):
                if v not in adjacency_list[u]:
                    disconnected_pair = (u, v)
                    break
            if disconnected_pair:
                break
        
        if disconnected_pair:
            f.write(f"{disconnected_pair[0]} {disconnected_pair[1]}\n")
        else:
            # Если все вершины соединены (граф полностью связный), выбираем случайную пару
            u, v = random.sample(range(1, n + 1), 2)
            f.write(f"{u} {v}\n")

        # 2 и 3. Запросы на соединённые вершины
        connected_pairs = set()
        
        def dfs_find_path(start, visited, target):
            """Поиск пути между двумя вершинами для генерации запросов."""
            if start == target:
                return True
            visited.add(start)
            for neighbor in adjacency_list[start]:
                if neighbor not in visited:
                    if dfs_find_path(neighbor, visited, target):
                        return True
            return False

        while len(connected_pairs) < 2:
            u = random.randint(1, n)
            v = random.randint(1, n)
            if u != v and dfs_find_path(u, set(), v):
                connected_pairs.add((u, v))
        
        for u, v in connected_pairs:
            f.write(f"{u} {v}\n")

# Пример использования
# n = 10  # Количество вершин
# m = 15  # Количество рёбер
n = int(input("n ="))
m = int(input("m = "))
test_num = int(input("test num = "))

generate_graph_tests(f"tests/{test_num}.txt", n, m)
