# create Property program.
# this is made to create every card that would be used in the game.
# this way the main program won't have to create the cards each time.
# the __init__ of this class should ALWAYS be the same as the one in the main program.
# if any changes take place in the main program to the properties class so should in this one.
import pickle
import os.path
from tkinter import *
from tkinter import ttk
import tkinter.messagebox
class Property():
	def __init__(self,name,colour,position,price,rent,house_1,house_2,house_3,house_4,hotel,image_path):
		self.name = name
		self.colour = colour
		self.position = position
		self.price = price
		self.house_price = int(position/10)*50+50 #house price should be 50,100,150,200 to not pass it as parameter it can be worked out using this formula
		self.house_lvl = 0
		self.price_per_stay_at_rent = rent
		self.price_per_stay_at_1 = house_1
		self.price_per_stay_at_2 = house_2
		self.price_per_stay_at_3 = house_3
		self.price_per_stay_at_4 = house_4
		self.price_per_stay_at_hotel = hotel
		self.owner = None
		self.mortgage = False
		self.image_path = image_path

	def __str__ (self):
		#two different messages because if the property is less than 10 it takes 1 digit space, if more then 2 digits, to keep consistency if it is less than 10, add a space before it.
		if self.position < 10:
			message = str(" " + str(self.position) + " " + self.name)
			return message
		message = str(str(self.position) + " " +self.name)
		return message

# names_list is the list that contains the names of the properties.
names_list = ["South Korea","Israel","Train Station","THE ANGEL ISLINGTON","EUSTON ROAD","PENTONVILLE ROAD","Japan","Power Station","Finland","Sweden","Train Station","BOW STREET","Germany","Austria","Canada","Belgium","TRAFALGAR SQUARE","Train Station","Austria","Gulag","The Three Gorges Dam","Netherlands","Norway","OXFORD STREET","Switzerland","Train Station","United Kingdom","Luxemburg"]
base = os.getcwd() #This gets the path of this program.
image_path = base + "\\Images\\Property" #This sets the path to the image folder
#Creating properties
#Property_n (n = position)names_list[x](x = pos in name list)	colour			position	price	rent	1house	2houese 3houses 4houses hotel	(base	   +		 image))
Property_1 = Property(names_list[0],							"Brown",		1,			60,		2,		10,		30,		90,		160,	250,	(image_path+"\\PROPERTY 1.png"))
Property_3 = Property(names_list[1],							"Brown",		3,			80,		4,		20,		60,		180,	320,	450,	(image_path+"\\PROPERTY 3.png"))
Property_5 = Property(names_list[2],							"Gray",			5,			200,	"other",25,		50,		100,	200,	None,	(image_path+"\\PROPERTY 5.png"))
Property_6 = Property(names_list[3],							"Light Blue",	6,			100,	6,		30,		90,		270,	400,	550,	(image_path+"\\PROPERTY 6.png"))
Property_8 = Property(names_list[4],							"Light Blue",	8,			100,	6,		30,		90,		270,	400,	550,	(image_path+"\\PROPERTY 8.png"))
Property_9 = Property(names_list[5],							"Light Blue",	9,			120,	8,		40,		100,	300,	450,	600,	(image_path+"\\PROPERTY 9.png"))
Property_11 = Property(names_list[6],							"Pink",			11,			140,	10,		50,		150,	450,	625,	750,	(image_path+"\\PROPERTY 11.png"))
Property_12 = Property(names_list[7],							"White",		12,			150,	"other",None,	None,	None,	None,	None,	(image_path+"\\PROPERTY 12.png"))
Property_13 = Property(names_list[8],							"Pink",			13,			140,	10,		50,		150,	450,	625,	750,	(image_path+"\\PROPERTY 13.png"))
Property_14 = Property(names_list[9],							"Pink",			14,			160,	12,		60,		180,	500,	700,	900,	(image_path+"\\PROPERTY 14.png"))
Property_15 = Property(names_list[10],							"Gray",			15,			200,	"other",25,		50,		100,	200,	None,	(image_path+"\\PROPERTY 15.png"))
Property_16 = Property(names_list[11],							"Orange",		16,			180,	14,		70,		200,	550,	700,	900,	(image_path+"\\PROPERTY 16.png"))
Property_18 = Property(names_list[12],							"Orange",		18,			180,	14,		70,		200,	550,	700,	900,	(image_path+"\\PROPERTY 18.png"))
Property_19 = Property(names_list[13],							"Orange",		19,			200,	16,		80,		220,	600,	800,	1000,	(image_path+"\\PROPERTY 19.png"))
Property_21 = Property(names_list[14],							"Red",			21,			220,	18,		90,		250,	700,	875,	1050,	(image_path+"\\PROPERTY 21.png"))
Property_23 = Property(names_list[15],							"Red",			23,			220,	18,		90,		250,	700,	875,	1050,	(image_path+"\\PROPERTY 23.png"))
Property_24 = Property(names_list[16],							"Red",			24,			240,	20,		100,	300,	750,	925,	1100,	(image_path+"\\PROPERTY 24.png"))
Property_25 = Property(names_list[17],							"Gray",			25,			200,	"other",25,		50,		100,	200,	None,	(image_path+"\\PROPERTY 25.png"))
Property_26 = Property(names_list[18],							"Yellow",		26,			260,	22,		110,	330,	800,	975,	1150,	(image_path+"\\PROPERTY 26.png"))
Property_27 = Property(names_list[19],							"Yellow",		27,			260,	22,		110,	330,	800,	975,	1150,	(image_path+"\\PROPERTY 27.png"))
Property_28 = Property(names_list[20],							"White",		28,			150,	"other",None,	None,	None,	None,	None,	(image_path+"\\PROPERTY 28.png"))
Property_29 = Property(names_list[21],							"Yellow",		29,			280,	24,		120,	360,	850,	1025,	1200,	(image_path+"\\PROPERTY 29.png"))
Property_31 = Property(names_list[22],							"Green",		31,			300,	26,		130,	390,	900,	1100,	1275,	(image_path+"\\PROPERTY 31.png"))
Property_32 = Property(names_list[23],							"Green",		32,			300,	26,		130,	390,	900,	1100,	1275,	(image_path+"\\PROPERTY 32.png"))
Property_34 = Property(names_list[24],							"Green",		34,			320,	28,		150,	450,	1000,	1200,	1400,	(image_path+"\\PROPERTY 34.png"))
Property_35 = Property(names_list[25],							"Gray",			35,			200,	"other",25,		50,		100,	200,	None,	(image_path+"\\PROPERTY 35.png"))
Property_37 = Property(names_list[26],							"Blue",			37,			360,	35,		175,	500,	1100,	1300,	1500,	(image_path+"\\PROPERTY 37.png"))
Property_39 = Property(names_list[27],							"Blue",			39,			400,	50,		200,	600,	1400,	1700,	2000,	(image_path+"\\PROPERTY 39.png"))

#Save the new created cards into file in Save0 folder
path_save = base +"\\Saved Games\\Slot 0"
board_position_list = ["GO",Property_1,"chest/chance",Property_3,"pay taxes",Property_5,Property_6,"chest/chance",Property_8,Property_9,"Gulag",Property_11,Property_12,Property_13,Property_14,Property_15,Property_16,"chest/chance",Property_18,Property_19,"Free Parking",Property_21,"chest/chance",Property_23,Property_24,Property_25,Property_26,Property_27,Property_28,Property_29,"Go to Jail",Property_31,Property_32,"chest/chance",Property_34,Property_35,"chest/chance",Property_37,"Pay Tax",Property_39]
#board position list is the array that stores every possible place on the board
file = open(path_save+"\\board position list.dat","wb") #open the file
pickle.dump(board_position_list,file) #save in the file
file.close() #close the file

#creating card class that is used as a parent for chest and chance cards
class Card():
	def __init__(self,effect,value,all_players_affected,image_path):
		self.effect = effect
		""" effect is what the card does this can be of  types:
			advance to ,includes jail(go to the specific position map)
			collect (money from a player or bank)
			pay (to a player or bank)
			move (go back 3 spaces), pay players, pay for houses
			make repairs (make general repairs on all your Property)"""
		self.value = value
		# value is either the amount to pay/received or position to go to
		self.all_players_affected = all_players_affected
		# players affected can be either True(everyone except the person drawing the card)False(only the current player)
		self.image_path = image_path
	def use_card(self,player_using,window):
		#the procedure that should be called when the card is drawn
		display_card(self,window) #firstly display the card for the player
		#then using selection find what is the effect of the card
		if self.effect == "advance to":
			#if it is advance then move to the specified position taking money from go
			if self.value == 30:
				#unless it is jail then set the position to 30 so it goes straight to jail
				player_using.set_position(self.value)
				return
			#simply to calculate if the player passes go just make the player move the set amount of steps
			player_position = player_using.get_position()
			spaces_to_move = self.value - player_position
			if spaces_to_move < 0:
				spaces_to_move += 40 #so if less than 0 (going backwards) add 40 (a go around the table) and make him move those places.
			player_using.move(spaces_to_move)
		elif self.effect == "receive" :
			player_using.give_money(self.value)
		elif self.effect == "pay" :
			if self.players_affected: #if True
				ID = "all"
			else:
				ID = None
			player_using.pay(self.value,ID)
		elif self.effect == "move": #this is used for move 3 places back.
			player_using.move(self.value)
		elif self.effect == "get out of jail":
			player_using.add_to_inventory(self)
			if self.value == "chest":
				chest_card_list.remove(self)
			else:
				chance_card_list.remove(self)
				#remove the card form the list so it can't have duplicates
		elif self.effect == "make repairs":
			amount_to_pay_for_houses = player_using.get_number_of_houses() * (25+self.value)
			amount_to_pay_for_hotels = player_using.get_number_of_hotels() * (100+self.value)
			amount_to_pay = amount_to_pay_for_hotels + amount_to_pay_for_houses
			player_using.pay(amount_to_pay,None)
	def display_card(self,window):
		def cant_exit():
			tkinter.messagebox.showinfo("Impossible to exit","Please use the \"OK\" button to exit the window")
		#creates a new window displaying the card
		card_image_window = Toplevel(window) #create the window (child of the pervious window)
		card_image_window.grab_set() #grab it (prioritize it)
		card_image_window.title(self.effect.capitalize()) #set the title of the effect with capitalizing first letter
		card_image_window.resizable(width=False, height=False) #maek the window set size (the size of the pictures)
		card_image_window.geometry("375x225")
		card_image = PhotoImage(file = self.image_path) #get the image path
		card_image_label = Label(card_image_window, image = card_image) #make the label with the image
		card_image_label.place(x=0, y=0, relwidth=1, relheight=1) #place that image
		pressedVar = BooleanVar() #create a var, it won't store anything it is just so the program will wait until player will press ok
		card_image_button = ttk.Button(card_image_window, text = "OK",command = lambda: pressedVar.set(True)) #create the button
		card_image_button.place(x=5,y=195) #place it in the bottom left
		card_image_window.protocol("WM_DELETE_WINDOW", cant_exit)
		card_image_button.wait_variable(pressedVar) #make the player wait until button pressed
		card_image_window.destroy() #destroy window
	def __str__(self):
		return self.description
#two different children classes for chest and chance cards
class Chest_Card(Card):
	global chest_card_list
	chest_card_list = [] #list of the chest cards
	def __init__(self,effect,value,all_players_affected,image_path):
		Card.__init__(self,effect,value,all_players_affected,image_path)
		chest_card_list.append(self) 

class Chance_Card(Card):
	global chance_card_list
	chance_card_list = [] #list of the chance cards
	def __init__(self,effect,value,all_players_affected,image_path):
		Card.__init__(self,effect,value,all_players_affected,image_path)
		chance_card_list.append(self)

#use this code to find path for the images
#base was used when creating properties, gets the file location of the running program.
cards_image_path = base + "\\Images\\Chest&Chance"

#creating cards
#creating chance cards
#chance_card_n = Chance_Card(effect,			value,		players_affected,	(base			 +			 image))
chance_card_1 = Chance_Card("collect",			50, 		False,				(cards_image_path+"\\Chance 1.png"))
chance_card_2 = Chance_Card("get out of jail",	"chance",	False,				(cards_image_path+"\\Chance 2.png"))
chance_card_3 = Chance_Card("pay",				15,			False,				(cards_image_path+"\\Chance 3.png"))
chance_card_4 = Chance_Card("advance to",		21,			False,				(cards_image_path+"\\Chance 4.png"))
chance_card_5 = Chance_Card("collect",			150,		False,				(cards_image_path+"\\Chance 5.png"))
chance_card_6 = Chance_Card("advance to",		30, 		False,				(cards_image_path+"\\Chance 6.png"))
chance_card_7 = Chance_Card("advance to",		39, 		False,				(cards_image_path+"\\Chance 7.png"))
chance_card_8 = Chance_Card("advance to",		11, 		False,				(cards_image_path+"\\Chance 8.png"))
chance_card_9 = Chance_Card("advance to",		40, 		False,				(cards_image_path+"\\Chance 9.png"))
chance_card_10 = Chance_Card("pay",				50,			True,				(cards_image_path+"\\Chance 10.png"))
chance_card_11 = Chance_Card("advance to",		6,			False,				(cards_image_path+"\\Chance 11.png"))
chance_card_12 = Chance_Card("move",			-3,			False,				(cards_image_path+"\\Chance 12.png"))
chance_card_13 = Chance_Card("advance to",		"train",	False,				(cards_image_path+"\\Chance 13.png"))
chance_card_14 = Chance_Card("advance to",		"train",	False,				(cards_image_path+"\\Chance 14.png"))
chance_card_15 = Chance_Card("advance to",		"utility",	False,				(cards_image_path+"\\Chance 15.png"))
chance_card_16 = Chance_Card("make repairs",	 0,			False,				(cards_image_path+"\\Chance 16.png"))

#creating chest cards
#chest_card_n = Chest_Card(effect,				value,		players_affected,	(base			 +	   image_path))
chest_card_1 = Chest_Card("pay",				50,			False,				(cards_image_path+"\\Chest 1.png"))
chest_card_2 = Chest_Card("pay",				100,		False,				(cards_image_path+"\\Chest 2.png"))
chest_card_3 = Chest_Card("get out of jail",	"chest",	False,				(cards_image_path+"\\Chest 3.png"))
chest_card_4 = Chest_Card("collect",			50,			True,				(cards_image_path+"\\Chest 4.png"))
chest_card_5 = Chest_Card("collect",			100,		False,				(cards_image_path+"\\Chest 5.png"))
chest_card_6 = Chest_Card("collect",			25,			False,				(cards_image_path+"\\Chest 6.png"))
chest_card_7 = Chest_Card("collect",			20,			False,				(cards_image_path+"\\Chest 7.png"))
chest_card_8 = Chest_Card("collect",			45,			False,				(cards_image_path+"\\Chest 8.png"))
chest_card_9 = Chest_Card("pay",				150,		False,				(cards_image_path+"\\Chest 9.png"))
chest_card_10 = Chest_Card("make repairs",		15,			False,				(cards_image_path+"\\Chest 10.png"))
chest_card_11 = Chest_Card("collect",			200,		False,				(cards_image_path+"\\Chest 11.png"))
chest_card_12 = Chest_Card("advance to",		40,			False,				(cards_image_path+"\\Chest 12.png"))
chest_card_13 = Chest_Card("collect",			100,		False,				(cards_image_path+"\\Chest 13.png"))
chest_card_14 = Chest_Card("collect",			100,		False,				(cards_image_path+"\\Chest 14.png"))
chest_card_15 = Chest_Card("collect",			10,			False,				(cards_image_path+"\\Chest 15.png"))
chest_card_16 = Chest_Card("advance to",		30,			False,				(cards_image_path+"\\Chest 16.png"))

#Save list of cards in Save0 folder
#saving the chance cards
file = open(path_save+"\\chance cards.dat","wb") #open the file
pickle.dump(chance_card_list,file) #save in the file
file.close()
#saving the chest cards
file = open(path_save+"\\chest cards.dat","wb") #open the file
pickle.dump(chest_card_list,file) #save in the file
file.close()

#close the program with a message
#you cannot use a message box on its own and the program has to create the window to show the message and then quickly destroy it
root = Tk()
#chance_card_14.display_card(root) #debug code to see the card display
root.withdraw()
tkinter.messagebox.showinfo("Success","Files have been saved successfully!")
root.destroy()