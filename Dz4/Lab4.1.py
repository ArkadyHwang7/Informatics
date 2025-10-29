def ArrayB(a):
    b = []
    for i in range(len(a)):
        if 10 < int(a[i]) < 50:
            b.append(a[i])
    return b

a = input("Введите 10 элементов массива A: ").split()
print("Элементы массива A:", a)
print("Элементы массива B (меньше 50 и больше 10):", ArrayB(a))