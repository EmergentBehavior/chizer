import unittest
import chizer

class TestChizer(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_isFileEncrypted(self):
        self.assertTrue(chizer.isFileEncrypted("data/faked.dat"),"file encryption check test for faked data")
        self.assertFalse(chizer.isFileEncrypted("data/Storj - Decentralizing Cloud Storage-vl3bUzfn2lg.mp4.gz"),"file encryption check test for gzipped move file")

    def test_isChunksEncrypted(self):
        self.assertFalse(chizer.isChunksEncrypted("data/faked.dat"),"chunks encryption check test for faked data")
        self.assertTrue(chizer.isChunksEncrypted("data/ac59ab5a282afd3de22062c7d62b5367"),"chuns encryption check test for encrypted file")

if __name__ == '__main__':
    unittest.main()