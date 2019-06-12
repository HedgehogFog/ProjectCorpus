class DataAddon(object):
    def __new__(cls):
        if not hasattr(cls, 'instance'):
            cls.instance = super(DataAddon, cls).__new__(cls)
        return cls.instance

        text = ""