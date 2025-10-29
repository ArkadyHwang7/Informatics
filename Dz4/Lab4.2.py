def intersection(a, b):
    inter = a & b
    total = sum(map(int, inter))
    return inter, total

inputA = input().split()
setA = set(inputA)
inputB = input().split()
setB = set(inputB)

inter, summa_inter = intersection(setA, setB)

# Вывод
print("Пересечение множеств:", inter)
print("Сумма элементов пересечения:", summa_inter)