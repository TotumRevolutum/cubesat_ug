import sys
from PyQt5 import QtGui, QtCore, QtWidgets
from PyQt5.QtWidgets import QWidget, QApplication, QPushButton
from PyQt5.QtWidgets import QInputDialog, QLabel
from PyQt5.QtGui import QPixmap


class Names(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.secondWin = None
        self.first = 'Первый'
        self.second = 'Второй'

    def initUI(self):
        fon_pic = QPixmap("pictures/sand.jpg")
        fon = QLabel(self)
        fon.setPixmap(fon_pic)
        fon.move(0, 0)

        self.setGeometry(300, 300, 1000, 1000)
        self.setWindowIcon(QtGui.QIcon("label.png"))

        self.label = QLabel(self)
        self.label.setText("Добро пожаловать в")
        self.label.move(390, 20)

        pic = QPixmap("label.png")
        lbl_pic = QLabel(self)
        lbl_pic.setPixmap(pic)
        lbl_pic.move(530, 14)

        pic_first = QPixmap("pictures/people.png")
        lbl_pic_first = QLabel(self)
        lbl_pic_first.setPixmap(pic_first)
        lbl_pic_first.move(100, 300)

        self.label2 = QLabel(self)
        self.label2.setText('Представьте, что вы перенеслись во '
                            'времени на 200 тысяч лет назад и '
                            'единственным развлечением для вас является'
                            ' "перекладывание камешков".\n')
        self.label2.move(30, 60)

        self.button_0 = QPushButton(self)
        self.button_0.move(412, 120)
        self.button_0.setText("Правила игры")
        self.button_0.clicked.connect(self.run0)

        self.button_1 = QPushButton(self)
        self.button_1.move(390, 160)
        self.button_1.setText("Играть против друга")
        self.button_1.clicked.connect(self.run1)

        self.button_2 = QPushButton(self)
        self.button_2.move(370, 200)
        self.button_2.setText("Играть против компьютера")
        self.button_2.clicked.connect(self.run2)

        self.label_name1 = QLabel(self)
        self.label_name1.move(380, 380)
        self.label_name1.setText('                                   ')

        self.label_name2 = QLabel(self)
        self.label_name2.move(540, 380)
        self.label_name2.setText('                                   ')

        pic_rock = QPixmap("pictures/huge_rock.png")
        lbl_pic_rock = QLabel(self)
        lbl_pic_rock.setPixmap(pic_rock)
        lbl_pic_rock.move(425, 650)

        self.btn = QPushButton('START', self)
        self.btn.move(450, 700)
        self.btn.clicked.connect(start_game)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    st = Names()
    st.show()
    sys.exit(app.exec_())