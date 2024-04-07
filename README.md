This is a CLI game I made for my college practice task
Task:
Создать набор классов, описывающих сущности звездного флота Империи.
Каждый класс должен описывать корабли следующих типов: фрегат, броненосец, эсминец, линкор. Характеристики корабля: наименование, максимальная скорость, мощность брони (в условных единицах), мощность силового поля (в условных единицах), количество членов экипажа, текущее положение в пространстве (координаты по x, y, z), а также вооружение. 
Вооружение также необходимо реализовать в виде отдельных классов: ракетные установки, лазеры, турели. 
Характеристики ракетной установки: количество ракет, скорострельность, огневая мощь одной ракеты (в усл.ед.), скорость ракеты.
Характеристики лазера: скорострельность, огневая мощь (в усл.ед.).
Характеристики турели: количество снарядов, скорострельность, огневая мощь (в усл.ед.), скорость снаряда.
Вооружение размещается на корабле. Соответственно, в зависимости от типа корабля его вооружение будет различным. Например, фрегат - небольшой корабль, поэтому имеет одну турель и лазер. Броненосец имеет 8 турелей. Эсминец имеет 8 ракетных установок. А линкор как самый большой корабль имеет n-ое количество ракетных установок, лазеров и турелей.
Для кораблей предусмотреть действия: перелет на указанное расстояние, ведение огня (с реализацией функционала истощения боезапаса, а также учета скорострельности и др. параметров), обработка попадания (с учетом огневой мощи снаряда, величины силового поля, величины брони).
Создать несколько экземпляров космических кораблей, разместить на позициях, смоделировать короткий бой с выводом действий и результатов действий (перемещение, выстрел, попадание, уничтожение).
