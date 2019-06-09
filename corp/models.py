from django.db import models #Импортируем модели Django


#Объект "Авторы"
class Author(models.Model):
    class Meta:
        verbose_name        = "Автор"
        verbose_name_plural = "Авторы"
    id_author = models.AutoField(primary_key  = True,
                                 verbose_name = "ID автора")
    name        = models.CharField(max_length=100, verbose_name="Имя автора")
    lastname    = models.CharField(max_length=100, verbose_name="Фамилия автора")
    middlename  = models.CharField(max_length=100, verbose_name="Отчество автора")

    def __str__(self):
        return self.name + " " + self.lastname + " " + self.middlename;

#Объект "Тема"
class Theme(models.Model):
    class Meta:
        verbose_name        = "Тема текста"
        verbose_name_plural = "Темы текстов"
    id_theme = models.AutoField(primary_key  = True,
                                verbose_name = "ID темы")
    theme     = models.CharField(max_length=100, verbose_name="Название темы")

    def __str__(self):
        return self.theme;

def addon_manual_path(instance, filename):
    return 'addons/{0}/manual/{1}'.format(instance.name, filename)

def addon_descr_path(instance, filename):
    return 'addons/{0}/description/{1}'.format(instance.name, filename)


def addon_example_path(instance, filename):
    return 'addons/{0}/example/{1}'.format(instance.name, filename)


#Объект "Анализаторов"
class Addon(models.Model):
    class Meta:
        verbose_name        = "Аддон"
        verbose_name_plural = "Аддоны"
    id_addon = models.AutoField(primary_key  = True,
                                verbose_name = "ID аддона")
    id_user         = models.ForeignKey('auth.User', db_column='id', verbose_name="ID юзера", on_delete = None)
    name            = models.CharField(max_length=100, verbose_name="Название аддона")
    description     = models.CharField(max_length=255, verbose_name="Краткое описание аддона")

    file_manual     = models.FileField(upload_to=addon_manual_path,     verbose_name="Инструкция")
    file_description= models.FileField(upload_to=addon_descr_path,      verbose_name="Описание файлов")
    file_example    = models.FileField(upload_to=addon_example_path,    verbose_name="Примеры использования")
    def __str__(self):
        return self.name;

#Объект "Заголовок"
class Header(models.Model):
    #Метаданные (текстовые описания)
    class Meta:
        verbose_name = "Заголовок корпуса"
        verbose_name_plural = "Заголовки корпусов"
    id_corp = models.AutoField(primary_key=True, verbose_name="ID корпуса")
    id_user     = models.ForeignKey('auth.User', db_column='id', verbose_name="ID юзера", on_delete = None, default=0)
    #Автоинкрементное поле. primary_key - первичный ключ таблицы, verbose_name - тестовое описание
    name = models.CharField(max_length=100, verbose_name="Название корпуса")
    #Текстовое поле. max_length - длина.
    type = models.CharField(max_length=100, verbose_name="Тип корпуса")
    created_date = models.DateTimeField(auto_now_add=True, verbose_name="Дата создания")
    visibility   = models.CharField(max_length = 1, verbose_name="Видимость корпуса", default=1)
    #Поле дата/время. auto_now_add - автоматическая установка текущей даты и времени при создании объекта.
    def __str__(self):
        return self.name

def texts_path(instance, filename):
    #Функция для получения пути файла для сохранения
    return 'texts/{0}/{1}'.format(instance.id_corp, filename)


#Объект "Позиция"
class Item(models.Model):
    class Meta:
        verbose_name = "Позиция корпуса"
        verbose_name_plural = "Позиции корпусов"
    id_item = models.AutoField(primary_key=True, verbose_name="ID текста")
    # id_corp = models.ForeignKey(Header, db_column='id_corp', verbose_name="ID корпуса", on_delete=models.CASCADE)
    id_corp  = models.ManyToManyField(Header, db_column='id_corp')
    id_theme = models.ForeignKey(Theme, db_column='id_theme', verbose_name="ID темы", on_delete = None, default = 0)

    #Поле внешнего ключа. Ссылается на модель Header, столбец "id_corp"(db_column). Задано каскадное удаление.
    title = models.CharField(max_length=100, verbose_name="Название текста")
    author = models.CharField(max_length=100, verbose_name="Автор текста")
    date = models.DateField(verbose_name="Дата текста")
    #Поле типа дата
    created_date = models.DateTimeField(auto_now_add=True, verbose_name="Дата загрузки")
    file = models.FileField(upload_to=texts_path, verbose_name="Текст")
    #Поле типа файл. Содержит в себе путь до файла с текстом на сервере.

    def __str__(self):
        return self.title