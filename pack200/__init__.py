import zipfile
import struct


class Unpacker:
    """
    Designed to read a Pack200 file
    """
    def __init__(self, filepath):
        self.filepath = filepath

    def _get_headers(self):
        """
        Gets the headers of the pack file
        :return: ??
        """
