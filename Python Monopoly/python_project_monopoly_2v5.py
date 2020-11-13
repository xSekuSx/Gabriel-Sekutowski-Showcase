#importing different modules
#they are used to: randomize, do windows buttons etc. ,save and load, get time from the system.
import random
from tkinter import *
from tkinter import ttk
import tkinter.messagebox
import pickle
import os.path
import time
import math
class player():
#template that represents the actual player that would play monopoly (position money etc.)
	global players
	players = []
	def __init__(self,ID,name,colour):
		self.ID = ID
		self.name = name
		self.colour = colour
		self.money = 2000
		self.position = 0
		self.bankruptcy = False
		self.turns_in_jail = 0
		self.properties_owned = []
		self.inventory = []
		#inventory is used to keep jail free cards
		self.owned_houses = 0
		self.owned_hotels = 0
		self.pawn = None
		self.region=2
		self.pawn_further = False
		self.jail_coords = []
		players.append(self)


	def get(self,info):
		if info == "ID":
			return self.ID
		elif info == "owned properties":
			return self.properties_owned
		elif info == "name":
			return self.name
		elif info == "colour":
			return self.colour
		elif info == "money":
			return self.money
		elif info == "position":
			return self.position
		elif info == "turns in jail":
			return self.turns_in_jail
		elif info == "get total houses":
			return self.owned_houses
		elif info == "get total hotels":
			return self.owned_hotels
		elif info == "total":
			total = self.money
			for i in range(len(self.properties_owned)):
				if self.properties_owned[i].house_lvl > 0:
					total += self.properties_owned[i].house_price * self.properties_owned[i].house_lvl
				total += self.properties_owned[i].price
			return total

	def move_player(self,amount_of_spaces,board):
		self.position += amount_of_spaces
		pawn_coords = board.coords(self.pawn)
		if amount_of_spaces == -40: #using -40  is toreset the the value but do not make the pawn go back 40 places
			return
		elif amount_of_spaces == -20:
			board.coords(self.pawn,self.jail_coords[0],self.jail_coords[1])
			self.region = 2
			self.position = 10
			return
		for i in range(amount_of_spaces):
			pawn_coords = board.coords(self.pawn)
			xdir = 0
			ydir = 0
			#time.sleep(0.5)
			#testing for region 0
			if pawn_coords[0] < 865 and pawn_coords[1] < 135:
				xdir = 80
				if self.region != 0:
					self.region = 0
					if self.pawn_further:
						xdir = 130
						ydir = -50
			#testing for region 1
			if pawn_coords[0] > 865 and pawn_coords[1] < 865:
				ydir = 80
				if self.region != 1:
					self.region = 1
					if self.pawn_further:
						xdir = 50
						ydir = 130
			#testing for region 2
			if pawn_coords[0] > 135 and pawn_coords[1] > 865:
				xdir = -80
				if self.region != 2:
					self.region = 2
					if self.pawn_further:
						xdir = -130
						ydir = 50
			#testing for region 3
			if pawn_coords[0] < 135 and pawn_coords[1] > 135:
				ydir = -80
				if self.region != 3:
					self.region = 3
					if self.pawn_further:
						xdir = -50
						ydir = -130
			#board.move(self.pawn,xdir,ydir)
			time.sleep(0.1)
			board.coords(self.pawn,(pawn_coords[0]+xdir),(pawn_coords[1]+ydir))
			board.update()



	def give_money(self,amount):
		self.money += amount

	def add_to_inventory(self,card):
		self.inventory.append(card)

	def pay(self,amount,ID):
		if ID == None:
			self.give_money(-amount)
			return
		elif ID == "all":
			for i in range(len(players)):
				players[i].give_money(amount)
			self.give_money(-amount*len(players))
			return
		self.give_money(-amount)
		players[ID-1].give_money(amount)

	def buy_property(self,current_property):
		self.pay(current_property.get("property price"),None)
		current_property.change_owner(self.ID)
		self.properties_owned.append(current_property)

	def sell_property(self,property_to_sell):
		self.properties_owned.remove(property_to_sell)

	def bankruptcy(self):
		self.bankruptcy = True

	def set_turns_in_jail(self,amount):
		self.turns_in_jail = amount

	def make_sell(self):
		#add this function later when buildings will be developed.
		"""to finish"""

	def add_to_inventory(self,item):
		self.inventory.append(item)

	def set_pawn(self,pawn,further,xjail,yjail):
		self.pawn = pawn
		self.pawn_further = further
		self.jail_coords.append(xjail)
		self.jail_coords.append(yjail)

	def __str__(self):
		message = str(self.ID) +" "+ self.name +" "+ self.colour +" "+ str(self.money) +" "+str(self.position)
		return message

class Property():
	#Property template is supposed to resemble the property card that is used in monopoly
	#It should have the same attributes the normal card would have: name, colour cost etc.
	def __init__(self,name,colour,price,rent,house_1,house_2,house_3,house_4,hotel,position,image_path):
		self.image_path = image_path
		self.name = name
		self.colour = colour
		self.price = price
		self.position = position
		self.house_price = int(position/10)*50+50
		self.house_lvl = 0
		self.price_per_stay_at_rent = rent
		self.price_per_stay_at_1 = house_1
		self.price_per_stay_at_2 = house_2
		self.price_per_stay_at_3 = house_3
		self.price_per_stay_at_4 = house_4
		self.price_per_stay_at_hotel = hotel
		self.mortgage = False
		self.owner = None


	def get(self,info):
		if info == "property price":
			return self.price
		elif info == "owner":
			return self.owner
		elif info == "amount to pay stay":
			if self.house_lvl == 0:
				return self.price_per_stay_at_rent 
			if self.house_lvl == 1:
				return self.price_per_stay_at_1 
			if self.house_lvl == 2:
				return self.price_per_stay_at_2 
			if self.house_lvl == 3:
				return self.price_per_stay_at_3 
			if self.house_lvl == 4:
				return self.price_per_stay_at_4 
			if self.house_lvl == 5:
				return self.price_per_stay_at_hotel
		elif info == "position":
			return self.position
		elif info == "image path":
			return self.image_path
		elif info == "house lvl":
			return self.house_lvl
		elif info == "ID":
			if self.position < 10:
				message = str(" " + str(self.position) + " " + self.name)
				return message
			message = str(str(self.position) + " " +self.name)
			return message

	def find_colour(self,player_owning):
		if self.position == 12 or self.position == 28 or (self.position%10) == 5: #if station or factory
			return "you can't buy houses on train stations or factories"
		n_houses_set = 0
		owned_properties = player_owning.get("owned properties")
		if self.colour == "Brown" or self.colour == "Blue": #blue and brown are the only colours to have only 2 properties
			n_colourt_set = 2
		else:
			n_colourt_set = 3
		for i in range(len(owned_properties)): #traverse trough the list of owned properties
			if owned_properties[i].colour == self.colour: #if any of the properties match colour with the one the player want to build houses on
				if n_houses_set == 0:
					house1 = owned_properties[i] #then set it as variable house 1
				elif n_houses_set == 1:
					house2 = owned_properties[i] #if house one is set set it as house 2
				else:
					house3 = owned_properties[i] #if house two is also set, set it to house 3
				n_houses_set += 1 #increment n_houses_set by 1 each time a match was found
				if n_houses_set == n_colourt_set: #if the number of houses set matches with the number of properties for that colour stop looping
					if n_colourt_set == 2: #if the colour has only two properties set house 3 as house 1
						house3 = house1
					break #leave loop
		colour_set = [house1,house2,house3]
		return colour_set

	def buy_house(self,player_building):
		try:
			houses = self.find_colour(player_building)
			mean_house_lvl = int((houses[0].get("house lvl") + houses[1].get("house lvl") + houses[2].get("house lvl"))/3) #get the average of all the houses rounding down.
		except:
			return "You do not own all the properties of that colour" #if error occurred it was because a variable was not defined and it was because player didn't own the property.
		if player_building.get("money") < self.house_price: #get user's money to see if can pay for the house
			return "You do not have enough money"
		elif self.house_lvl != mean_house_lvl: #check if the player can build a house there
			return "You have to build houses in the properties where there's the lowest level of housing"
		elif self.house_lvl == 5: #check if he didn't each maximum level
			return "You have reached the highest housing level:Hotel"
		self.house_lvl += 1 #otherwise increase the house level
		player_building.pay(self.house_price,None) #and pay for the house
		if self.house_lvl == 5:
			player_building.owned_hotels += 1
		else:
			player_building.owned_houses += 1
		return "House Built" #return successful message

	def sell(self,player_selling):
		try: #when successful player owns all properties of that colour
			houses = self.find_colour(player_selling)
			max_house_lvl = max(houses[0].get("house lvl"), houses[1].get("house lvl"), houses[2].get("house lvl")) #finds the highest property level
			if max_house_lvl > 0:
				if self.house_lvl == max_house_lvl:
					self.house_lvl -= 1
					player_selling.give_money(self.house_price)
					return "House Sold"
				return "You have to sell houses on other properties first"
			self.owner = None
			player_selling.give_money(self.price)
			player_selling.properties_owned.remove(self)
			return "Property Sold"
		except: #player does not own every property of that colour
			self.owner = None
			player_selling.give_money(self.price)
			player_selling.properties_owned.remove(self)
			return "Property Sold"

	def morgage(self,player_mortgaging):
		if self.mortgage == True:
			return "Property already in mortgage"
		try:
			houses = self.find_colour(player_mortgaging)
			max_house_lvl = max(houses[0].get("house lvl"), houses[1].get("house lvl"), houses[2].get("house lvl"))
			if max_house_lvl > 0:
				return "You cannot mortgage a property with houses"
		except:
			pass
		self.mortgage = True
		player_mortgaging.give_money(int(self.price/2))
		return "Property Mortgaged"

	def unmortgage(self,player_unmortgaging):
		if self.mortgage == False:
			return "Property already is unmortgaged"
		if player_unmortgaging.get("money") < (self.price/2):
			return "You do not have enough money"
		self.mortgage = False
		player_unmortgaging.pay((int(self.price/2)),None)
		return "Property Unmogaged"

	def change_owner(self,playerID):
		self.owner = playerID

	def show_image_w_decision(self,window):
		property_image_window = Toplevel(window)
		property_image_window.grab_set()
		property_image = PhotoImage(file = self.image_path)
		property_image_label = Label(property_image_window, image = property_image)
		property_image_label.pack()
		decision = tkinter.messagebox.askquestion("Purchase","Do you want to buy this property?")
		property_image_window.destroy()
		return decision

	def __str__ (self):
		if self.position < 10:
			message = str(" " + str(self.position) + " " + self.name)
			return message
		message = str(str(self.position) + " " +self.name)
		return message

class Card():
	def __init__(self,effect,value,all_players_affected,image_path):
		self.effect = effect
		""" effect is what the card does this can be of  types:
			advance to ,includes jail(go to the specific position map)
			collect (from a player or bank)
			pay (to a player or bank)
			move (go back 3 spaces), pay players, pay for houses
			make repairs (make general repairs on all your Property)"""
		self.value = value
		# value is either the amount to pay/received or position to go to
		self.all_players_affected = all_players_affected
		# players affected can be either True(everyone except the person drawing the card)False(only the current player)
		self.image_path = image_path
	def use_card(self,player_using,window,card_list,board):
		#the procedure that should be called when the card is drawn
		self.display_card(window) #firstly display the card for the player
		#then using selection find what is the effect of the card
		if self.effect == "advance to":
			#if it is advance then move to the specified position taking money from go
			if self.value == 30:
				#unless it is jail then set the position to 30 so it goes straight to jail
				player_using.set_turns_in_jail(3)
				player_using.move_player(-20,board)
				return
			#simply to calculate if the player passes go just make the player move the set amount of steps
			player_position = player_using.get("position")
			try:
				spaces_to_move = self.value - player_position
				if spaces_to_move < 0:
					spaces_to_move += 40 #so if less than 0 (going backwards) add 40 (a go around the table) and make him move those places.
			except:
				if self.value == "train":
					spaces_to_move = player_position
					for i in range(10):
						if spaces_to_move % 10 == 5:
							break
						spaces_to_move += 1
					spaces_to_move -= player_position
				elif self.value == "utility":
					if player_position > 12 and player_position < 28:
						spaces_to_move = 28 - player_position
					else:
						if player_position > 28:
							spaces_to_move = 52 - player_position
						else:
							spaces_to_move = 12 - player_position
			player_using.move_player(spaces_to_move,board)
		elif self.effect == "collect" :
			player_using.give_money(self.value)
		elif self.effect == "pay" :
			if self.all_players_affected: #if True
				ID = "all"
			else:
				ID = None
			player_using.pay(self.value,ID)
		elif self.effect == "move": #this is used for move 3 places back.
			player_using.move_player(self.value,board)
		elif self.effect == "get out of jail":
			player_using.add_to_inventory(self)
			card_list.remove(self)
				#remove the card form the list so it can't have duplicates
		elif self.effect == "make repairs":
			amount_to_pay_for_houses = player_using.get("get total houses") * (25+self.value)
			amount_to_pay_for_hotels = player_using.get("get total hotels") * (100+self.value)
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

class Chest_Card(Card):
	chest_card_list = [] #list of the chest cards
	def __init__(self,effect,value,all_players_affected,image_path):
		Card.__init__(self,effect,value,all_players_affected,image_path)
		chest_card_list.append(self)

class Chance_Card(Card):
	chance_card_list = [] #list of the chance cards
	def __init__(self,effect,value,all_players_affected,image_path):
		Card.__init__(self,effect,value,all_players_affected,image_path)
		chance_card_list.append(self)

def random_digit(n_die,die_image_label):
	#I could be using a random digit when i call the function but later I will have tkinter called here to change die side.
	die = random.randint(1,6)
	if die == 1:
		die_image = PhotoImage(file = "Images\\Dice\\die 1.png")
	elif die == 2:
		die_image = PhotoImage(file = "Images\\Dice\\die 2.png")
	elif die == 3:
		die_image = PhotoImage(file = "Images\\Dice\\die 3.png")
	elif die == 4:
		die_image = PhotoImage(file = "Images\\Dice\\die 4.png")
	elif die == 5:
		die_image = PhotoImage(file = "Images\\Dice\\die 5.png")
	elif die == 6:
		die_image = PhotoImage(file = "Images\\Dice\\die 6.png")

	die_image_label.configure(image = die_image)
	die_image_label.image = die_image
	return die

def check_position(position):
	property_list = [1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,32,34,35,37,39]
	# fill this with all the positions where a property stands, and the others with other positions.
	chance_list = [7,22,36]
	chest_list = [2,17,33]
	if position in property_list:
		return "property"
	elif position in chance_list:
		return "chance"
	elif position in chest_list:
		return "chest"
	else:
		# it doesn't really matter what is returned as there is an else-if if they fail the comparisons above
		return

def property_management_window(player_managing,window,name_label,colour_label,position_label,money_label,purpose):
	#create a window where the player will choose and win buy houses
	#divide the window into two parts(frames) 
	#left side where the card of the selected property will be displayed
	#right one where the drop down menu and button will be placed
	#make the player select a property from a drop down menu

	def find_act_variable(player_managing,var_name):
		owned_properties = player_managing.get("owned properties")
		for i in range(len(owned_properties)):
			if owned_properties[i].get("ID") == var_name:
				return [owned_properties[i],i] #return the varaible and it's position in list

	def update_image(player_managing,image_label,label_to_update,purpose):
		#procedure to update the image in the build window to match the one selected in the drop down menu
		try:
			actual_variables = find_act_variable(player_managing,selected_property_name.get())
		except:
			return
		#get the image path of that variable and change it in the window
		current_property_image = PhotoImage(file = actual_variables[0].get("image path"))
		#this gets that label and uses configure to edit it's image value
		image_label.configure(image = current_property_image)
		image_label.image = current_property_image
		if purpose == "Build" or purpose =="Sell": #if any of these show house level
			lvl_message = "House level: " + str(actual_variables[0].get("house lvl"))
			label_to_update.configure(text = lvl_message)
		else: #otherwise show if in mortage
			mortgage_status_message = "Mortgage : " + str(actual_variables[0].mortgage)
			label_to_update.configure(text = mortgage_status_message)

	def build_houses(player_managing,name_label,colour_label,position_label,money_label,window_money_label,window_house_lvl_label):
		try:
			actual_variables = find_act_variable(player_managing,selected_property_name.get())
		except:
			return
		#use buy_house procedure on that variable
		message = actual_variables[0].buy_house(player_managing)
		#create a window to later display a message using tkinter showinfo messagebox
		update_interface(player_managing,name_label,colour_label,position_label,money_label)
		money_message = "Money: " + str(player_managing.get("money"))
		lvl_message = "House level: " + str(actual_variables[0].get("house lvl"))
		window_money_label.config(text = money_message)
		window_house_lvl_label.config(text = lvl_message)
		tkinter.messagebox.showinfo("Build house",message)
		#destroy the window (this will happen when the user will press ok on the messa_box)

	def sell_houses(player_managing,name_label,colour_label,position_label,money_label,window_money_label,window_house_lvl_label,image_label):
		try:
			actual_variables = find_act_variable(player_managing,selected_property_name.get())
		except:
			return
		message = actual_variables[0].sell(player_managing)
		update_interface(player_managing,name_label,colour_label,position_label,money_label)
		money_message = "Money: " + str(player_managing.get("money"))
		try:
			lvl_message = "House level: " + str(actual_variables[0].get("house lvl"))
		except:
			lvl_message = "House level: 0"
		window_money_label.config(text = money_message)
		window_house_lvl_label.config(text = lvl_message)
		if message == "Property Sold": #if the property was sold delete the instance in dropdown
			selected_property_name.set("Select Property")
			properties_dropdown['menu'].delete(actual_variables[1]) #delete ith item from dropdown which will be the selected one.
			# Insert list of new options (tk._setit hooks them up to var)
			base_path = os.getcwd() #get path of program to then get path of image
			prop_0_path = base_path + "\\Images\\Property\\PROPERTY 0.png" #replace the image with standard
			prop_0_image = PhotoImage(file = prop_0_path)
			image_label.configure(image = prop_0_image)
			image_label.image = prop_0_image
		tkinter.messagebox.showinfo("Sell house/property",message)

	def mortgage_property(player_managing,name_label,colour_label,position_label,money_label,window_money_label,property_mortgage_label):
		try:
			actual_variables = find_act_variable(player_managing,selected_property_name.get())
		except:
			return
		message = actual_variables[0].morgage(player_managing) #using method get message
		update_interface(player_managing,name_label,colour_label,position_label,money_label)
		money_message = "Money: " + str(player_managing.get("money"))
		mortgage_status_message = "Mortgage : " + str(actual_variables[0].mortgage) #get mortage message
		window_money_label.config(text = money_message)
		property_mortgage_label.config(text = mortgage_status_message) #update mortage label
		tkinter.messagebox.showinfo("Mortgage property",message)

	def unmortgage_property(player_managing,name_label,colour_label,position_label,money_label,window_money_label,property_mortgage_label):
		try:
			actual_variables = find_act_variable(player_managing,selected_property_name.get())
		except:
			return
		message = actual_variables[0].unmortgage(player_managing)
		update_interface(player_managing,name_label,colour_label,position_label,money_label)
		money_message = "Money: " + str(player_managing.get("money"))
		mortgage_status_message = "Mortgage : " + str(actual_variables[0].mortgage)
		window_money_label.config(text = money_message)
		property_mortgage_label.config(text = mortgage_status_message)
		tkinter.messagebox.showinfo("Unmortgage property",message)

	#create new window
	property_management_window = Toplevel(window)
	property_management_window.title(purpose)
	property_management_window.grab_set()

	#get the path to the images
	base_path = os.getcwd()
	prop_0_path = base_path + "\\Images\\Property\\PROPERTY 0.png"
	#create left frame (the one with the image)
	image_frame = Frame(property_management_window)
	image_frame.grid(row=0)
	#take picture of prop0 and place it there (use it as a place holder)
	property_image = PhotoImage(file = prop_0_path)
	property_image_label = Label(image_frame, image = property_image)
	property_image_label.grid(row=0)

	#create right frame (the one with drop down menu and button)
	buttons_dropdown_frame = Frame(property_management_window)
	buttons_dropdown_frame.grid(row=0, column=1)
	#split it into two frames once more
	drop_down_bar_frame = Frame(buttons_dropdown_frame)
	drop_down_bar_frame.grid(row=0)
	#make a list where the NAMES will be stored as strings, these will be displayed in the drop down menu
	owned_properties_strings = []
	#a for loop that goes trough every owned property and gets it's name
	for i in range(len(player_managing.get("owned properties"))):
		owned_properties_strings.append(player_managing.get("owned properties")[i])
	#create variable where the value from the drop down menu will be stored in
	selected_property_name = StringVar(property_management_window)
	selected_property_name.set("Select Property")
	#exception handling in case the player does not own the properties (list with the property names is empty)
	#if it is make it hold 1 item in list "no properties"
	#whenever the user chooses a new item from the menu (new name)
	#call the update image procedure
	if purpose == "Build" or purpose == "Sell":
		properties_dropdown = ttk.OptionMenu(drop_down_bar_frame,selected_property_name,selected_property_name.get(),*owned_properties_strings,command = lambda _: update_image(player_managing,property_image_label,property_management_house_lvl,purpose))
	else:
		properties_dropdown = ttk.OptionMenu(drop_down_bar_frame,selected_property_name,selected_property_name.get(),*owned_properties_strings,command = lambda _: update_image(player_managing,property_image_label,property_mortgage_label,purpose))
	properties_dropdown.grid(row=0)

	#create the second frame in right frame for the button
	buttons_frame = Frame(buttons_dropdown_frame)
	buttons_frame.grid(row=1)
	#its purpose is to call the build houses procedure
	if purpose == "Build":
		build_button = Button(buttons_frame,text = "Build A house" ,width = 20 ,command = lambda: build_houses(player_managing,name_label,colour_label,position_label,money_label,property_management_money_label,property_management_house_lvl,))
		build_button.grid(row=0)
	elif purpose == "Sell":
		sell_button = Button(buttons_frame,text = "Sell house/property" ,width = 20 ,command = lambda: sell_houses(player_managing,name_label,colour_label,position_label,money_label,property_management_money_label,property_management_house_lvl,property_image_label))
		sell_button.grid(row=0)
	elif purpose == "Mortgage":
		mortgage_button = Button(buttons_frame,text = "Mortgage property", width = 20 ,command = lambda: mortgage_property(player_managing,name_label,colour_label,position_label,money_label,property_management_money_label,property_mortgage_label))
		mortgage_button.grid(row=0)
	elif purpose == "Unmortgage":
		unmortgage_button = Button(buttons_frame,text = "Unmortgage property" ,width = 20 ,command = lambda: unmortgage_property(player_managing,name_label,colour_label,position_label,money_label,property_management_money_label,property_mortgage_label))
		unmortgage_button.grid(row=0)
	else:
		tkinter.messagebox.showinfo("Error","Error")

	info_frame = Frame(buttons_dropdown_frame)
	info_frame.grid(row=2)
	money_message = "Money: " + str(player_managing.get("money"))
	property_management_money_label = Label(info_frame,text = money_message)
	property_management_money_label.pack()
	if purpose == "Build" or purpose == "Sell":
		property_management_house_lvl = Label(info_frame,text = "House level: None")
		property_management_house_lvl.pack()
	else:
		property_mortgage_label = Label(info_frame,text = "Mortgage: None")
		property_mortgage_label.pack()
	property_management_window.mainloop()

def auction(position):
	#auction if someone doesn't buy the property
	return 

def update_interface(player_n,name_label,colour_label,position_label,money_label):
	#pass the labels as parameters and change their configures.
	#get the current player info
	name = player_n.get("name")
	colour = player_n.get("colour") 
	position = "position: " + str(player_n.get("position"))
	money = "money: " + str(player_n.get("money"))
	#change the value of the labels and their colour
	name_label.config(text = name,fg = colour)
	colour_label.config(text = colour,fg = colour)
	position_label.config(text = position,fg = colour)
	money_label.config(text = money, fg = colour)

def roll_dice(player_n,window,die_1_image_label,die_2_image_label,button_roll_dice,button_end_turn,board_position_list,name_label,colour_label,position_label,money_label,chance_list,chest_list,board):
	def property_tile(player_n,board_position_list,current_position,diceroll):
		current_property = board_position_list[current_position]
		owner = current_property.get("owner")
		if owner != None:
		# checks if the property has an owner
			amount = current_property.get("amount to pay stay")
			try:
				amount = amount + 0
			except:
				#the amount is not an integer therefore trains or utility
				list_owned_properties = players[owner - 1].get("owned properties") #get the list of owned properties of the player the current player has to pay
				if (current_position % 10) == 5: #it's a train square if it end in 5
					trains_owned = -1 #this has to be -1 for the amount calculator to work
					for i in range(len(list_owned_properties)): #use iteration to find the number of train stations owned.
						if (list_owned_properties[i].position % 10) == 5:
							trains_owned += 1 #if train station found add 1 to train station number
					amount = 25 * (2 ** trains_owned) #the values are 25,50,100,200 or (25*2^0, 25*2^1, 25*2^2, 25*2^3) that's why trains owned is set to -1
				else:
					utilities_owned = 0 #if it is not train then it has to be utilities
					for i in range (len(list_owned_properties)): #use similar aproach to find amount of utilities (there are 2 in total)
						if list_owned_properties[i].get("position") == 12 or list_owned_properties[i].get("position") == 28:
							utilities_owned += 1
					if utilities_owned == 1:
						amount = diceroll * 4 #normally you would roll again but because that would be very complicated I decided the program to use the current rolled amount.
					elif utilities_owned == 2:
						amount = diceroll * 10
					else:
						tkinter.messagebox.showinfo("error","utilities owned more than 2") #debug code
			if player_n.get("money") < amount:
				# see if player has enough money to pay
				if player_n.get("total") < amount:
					# see if player has enough money to pay even if everything is sold 
					player_n.bankruptcy()
					# if not make him a bankrupt
				# if the player cannot afford but his total money is enough make him sell the houses etc.
				player_n.make_sell()
				#this will be added later, not sure about the house values etc.
			player_n.pay(amount,owner)
		# all of this is skipped if the property is unowned
		# ask the player if wants to buy (check if has enough money) 
		property_cost = current_property.get("property price")
		if player_n.get("money") >= property_cost and owner == None:
			decision = current_property.show_image_w_decision(window)
			if decision == "no":
				auction(current_position)
			else:
				player_n.buy_property(current_property)
		else:
			auction(current_position)

	button_roll_dice["state"] = "disabled"
	button_end_turn["state"] = "normal"
	turns_in_jail = player_n.get("turns in jail")
	if turns_in_jail > 0:
		message = "You will be in Jail for next "+str(turns_in_jail)+" turn(s)"
		tkinter.messagebox.showinfo("You are in Jail",message)
		reduced_turns = turns_in_jail - 1
		player_n.set_turns_in_jail(reduced_turns)
		button_roll_dice["state"] = "disabled"
		button_end_turn["state"] = "normal"
		return
	die_1 = random_digit(0,die_1_image_label)
	die_2 = random_digit(1,die_2_image_label)
	diceroll = die_1 + die_2
	player_n.move_player(diceroll,board)
	if player_n.get("position") > 39:
		player_n.move_player(-40,board)
		player_n.give_money(200)
		# if player has passed trough go (position > 40 a.k.a. a lap) give 200
	current_position = player_n.get("position")
	update_interface(player_n,name_label,colour_label,position_label,money_label)
	square_standing = check_position(current_position)
	# check what the player is standing on (property, chest/chance or else)
	if square_standing == "property":
		property_tile(player_n,board_position_list,current_position,diceroll)
	elif square_standing == "chance" or square_standing == "chest":
		if square_standing == "chance":
			card_list = chance_list
		else:
			card_list = chest_list
		card = card_list.pop()
		card_list.insert(0,card)
		card.use_card(player_n,window,card_list,board)
		if player_n.get("position") > 39:
			player_n.move_player(-40,board)
			player_n.give_money(200)
		current_position = player_n.get("position")
		square_standing = check_position(current_position)
		if square_standing == "property":
			property_tile(player_n,board_position_list,current_position,diceroll)
		else:
			if current_position == 4 or current_position == 38:
				amount = 200
				if current_position == 38:
					amount = 100
					#change value to the one where you pay 100, this would reduce lines of code if i would want to use the same code but with different values.
				if player_n.get("money") < amount:
					# see if player has enough money to pay
					if player_n.get("total") < amount:
						# see if player has enough money to pay even if everything is sold 
						player_n.bankruptcy()
						# if not make him a bankrupt
					# if the player cannot afford but his total money is enough make him sell the houses etc.
					player_n.make_sell()
					#this will be added later, not sure about the house values etc.
				player_n.pay(amount,None)
	else:
		# this is when a player stops on a special square (free parking or else)
		# special are on 4,10,20,30,38 -> these are income tax, visit jail, free parking, go to jail, "super tax".
		if current_position == 4 or current_position == 38:
			amount = 200
			if current_position == 38:
				amount = 100
				#change value to the one where you pay 100, this would reduce lines of code if i would want to use the same code but with different values.
			if player_n.get("money") < amount:
				# see if player has enough money to pay
				if player_n.get("total") < amount:
					# see if player has enough money to pay even if everything is sold 
					player_n.bankruptcy()
					# if not make him a bankrupt
				# if the player cannot afford but his total money is enough make him sell the houses etc.
				player_n.make_sell()
				#this will be added later, not sure about the house values etc.
			player_n.pay(amount,None)
		if current_position == 30:
			player_n.set_turns_in_jail(3)
			player_n.move_player(-20,board)
	update_interface(player_n,name_label,colour_label,position_label,money_label)

def next_turn(button_roll_dice,button_end_turn,name_label,colour_label,position_label,money_label):
	global current_player
	global current_turn
	global players
	current_turn = current_turn + 1
	if current_turn == len(players):
		current_turn = current_turn - len(players)
	current_player = players[current_turn]
	button_roll_dice["state"] = "normal"
	button_end_turn["state"] = "disabled"
	update_interface(current_player,name_label,colour_label,position_label,money_label)

def option_quit(root):
	#the user wants to quit
	#destroy (exit) the window
	root.destroy()

def option_new(root):
	root.withdraw()
	#hide the menu window
	def player_creation(frame1,frame2,frame3,window,root):
		try:
			num_bots = number_of_bots.get() #if the program tries to recall the var "num_bots" if it empty the program outputs an error
			num_humans = number_of_humans.get() #the same thing here
			if (num_bots + num_humans) < 2:
				tkinter.messagebox.showinfo("select players","please select the number of computer and human players") #message for the user
				return #end function
		except:
			tkinter.messagebox.showinfo("select players","please select the number of computer and human players") #message for the user
			return #end function

		def update_button(button,status):
			button["state"] = status #change the state of the button this would be between enabled/disabled

		def update_colours(button,p_selected,p_previous_selected):
			"""print(p_previous_selected.get())
			#print(p_selected.get())""" #debug code to see player's current and previous colour
			if p_previous_selected.get() != "None" and p_previous_selected.get() != '':
				#if last option was a colour
				#add previous colour
				colours.append(p_previous_selected.get())
			try:
				if p_selected.get() != "None":
					#if the player selected a colour
					colours.remove(p_selected.get()) #try to remove the colour form the list
				p_previous_selected.set(p_selected.get())
			except: #the program couldn't remove the colour
				p_selected.set("None") #set the colour as none
				p_previous_selected.set("None")
				tkinter.messagebox.showinfo("Colour chosen","The colour is already chosen, please select another one")
			num_humans = number_of_humans.get()
			if (len(colours)-1) == (8 - num_humans): #check if all players have selected a colour
				button["state"] = "normal"  #change the button state to enabled
			else:
				button["state"] = "disabled"#else disable it

		#erases the previous frames
		frame1.pack_forget()
		frame2.pack_forget()
		frame3.pack_forget()
		#colours is the list that stores the possible colours the player can choose from
		global colours
		colours = ["None","green","red","yellow","blue","black","orange","pink","purple"]

		#creates 2 frames
		p_names_frame = Frame(window)
		p_names_frame.pack(side = "left", fill="both", expand="yes")
		p_colours_frame = Frame(window)
		p_colours_frame.pack(side = "left", fill="both", expand="yes")
		indicator_names_label = Label(p_names_frame,text = "Name")
		indicator_names_label.pack(fill="both", expand="yes")
		indicator_colours_label = Label(p_colours_frame,text = "Colour")
		indicator_colours_label.pack(fill="both", expand="yes")

		#declaring string vars and selected (colour) vars
		p1_nameVar = StringVar()
		p2_nameVar = StringVar()
		p3_nameVar = StringVar()
		p4_nameVar = StringVar()
		p5_nameVar = StringVar()
		p6_nameVar = StringVar()
		p7_nameVar = StringVar()
		p8_nameVar = StringVar()

		p1_selected = StringVar()
		p2_selected = StringVar()
		p3_selected = StringVar()
		p4_selected = StringVar()
		p5_selected = StringVar()
		p6_selected = StringVar()
		p7_selected = StringVar()
		p8_selected = StringVar()

		p1_previous_selected = StringVar()
		p2_previous_selected = StringVar()
		p3_previous_selected = StringVar()
		p4_previous_selected = StringVar()
		p5_previous_selected = StringVar()
		p6_previous_selected = StringVar()
		p7_previous_selected = StringVar()
		p8_previous_selected = StringVar()

		launch_frame = Frame(window) #last frame
		launch_frame.pack(side = "left", fill="both", expand="yes")
		launch_button = Button(launch_frame,text = "Launch", command = lambda: create_new_game(p1_nameVar,p2_nameVar,p3_nameVar,p4_nameVar,p5_nameVar,p6_nameVar,p7_nameVar,p8_nameVar,p1_selected,p2_selected,p3_selected,p4_selected,p5_selected,p6_selected,p7_selected,p8_selected,window,root), state = "disabled")
		launch_button.pack(fill = X,expand=True)

		def create_entry_dropdown(entry_frame,entryVar,dropdown_frame,p_selected,p_previous_selected,launch_button):
			def optionmenu_patch(optionmenu, var):
				#this procedure "separates" the dropdown menus
				menu = optionmenu['menu'] #opens dropdwon menu's menu
				last = menu.index("end") #gets last index
				for i in range(0, last+1): #loops trough the menu's to last element + 1
					menu.entryconfig(i, variable=var) #separates the elements
			Entry(entry_frame,textvariable = entryVar).pack(fill="both", expand="yes") #creates entry
			p_selected.set("None") 
			dropdown = ttk.OptionMenu(dropdown_frame,p_selected,colours[0],*colours, command = lambda _: update_colours(launch_button,p_selected,p_previous_selected)) #creates dropdown
			dropdown.pack(fill="both", expand="yes")
			optionmenu_patch(dropdown,p_selected) #calls the function to patch it

		create_entry_dropdown(p_names_frame,p1_nameVar,p_colours_frame,p1_selected,p1_previous_selected,launch_button)
		if num_humans > 1:
			create_entry_dropdown(p_names_frame,p2_nameVar,p_colours_frame,p2_selected,p2_previous_selected,launch_button)
		if num_humans > 2:
			create_entry_dropdown(p_names_frame,p3_nameVar,p_colours_frame,p3_selected,p3_previous_selected,launch_button)
		if num_humans > 3:
			create_entry_dropdown(p_names_frame,p4_nameVar,p_colours_frame,p4_selected,p4_previous_selected,launch_button)
		if num_humans > 4:
			create_entry_dropdown(p_names_frame,p5_nameVar,p_colours_frame,p5_selected,p5_previous_selected,launch_button)
		if num_humans > 5:
			create_entry_dropdown(p_names_frame,p6_nameVar,p_colours_frame,p6_selected,p6_previous_selected,launch_button)
		if num_humans > 6:
			create_entry_dropdown(p_names_frame,p7_nameVar,p_colours_frame,p7_selected,p7_previous_selected,launch_button)
		if num_humans > 7:
			create_entry_dropdown(p_names_frame,p8_nameVar,p_colours_frame,p8_selected,p8_previous_selected,launch_button)

	def create_new_game(p1_nV,p2_nV,p3_nV,p4_nV,p5_nV,p6_nV,p7_nV,p8_nV,p1_sl,p2_sl,p3_sl,p4_sl,p5_sl,p6_sl,p7_sl,p8_sl,window,root):
		#px_nV where x is number of the player nV stands for nameVariable
		#px_sl where x is number of the player sl stands for selected (colour)
		human_players = number_of_humans.get()
		player_1 = player(1,p1_nV.get(),p1_sl.get())
		stop = 1
		if human_players > 1:
			player_2 = player(2,p2_nV.get(),p2_sl.get())
			stop = 2
		if human_players > 2:
			player_3 = player(3,p3_nV.get(),p3_sl.get())
			stop = 3
		if human_players > 3:
			player_4 = player(4,p4_nV.get(),p4_sl.get())
			stop = 4
		if human_players > 4:
			player_5 = player(5,p5_nV.get(),p5_sl.get())
			stop = 5
		if human_players > 5:
			player_6 = player(6,p6_nV.get(),p6_sl.get())
			stop = 6
		if human_players > 6:
			player_7 = player(7,p7_nV.get(),p7_sl.get())
			stop = 7
		if human_players > 7:
			player_8 = player(8,p8_nV.get(),p8_sl.get())
			stop = 8
		bot_players = number_of_bots.get()
		#names list is used for the bots so that they have a name that represents them
		names = ["Bob","James","Tom","Adolf","Cyprian","Bryan","Martyna","Sam","Gabe","Lara","Borys","Wladimir","Sasha","Wojtyła","Vasiliy","Ivan","Siergiej"]
		colours.remove("None")
		random.shuffle(names)
		random.shuffle(colours)
		if bot_players > 0 and stop == 1:
			player_2 = player(2,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 2:
			player_3 = player(3,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 3:
			player_4 = player(4,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 4:
			player_5 = player(5,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 5:
			player_6 = player(6,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 6:
			player_7 = player(7,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		if bot_players > 0 and stop == 7:
			player_8 = player(8,names.pop(),colours.pop())
			bot_players -= 1
			stop += 1
		"""for i in range(len(players)):
			print(players[i])""" #debug code to see if players created
		window.destroy()
		start_game(root)

	def update_radio_buttons(buttons_to_update,list_disable):
		for i in range(buttons_to_update):
			#count trough the list and disable the buttons
			list_disable[i].configure(state = "disabled")
		list_enable = list_disable[::-1]
		#reverse the list
		for i in range(8-buttons_to_update):
			#count trough the list and enable the buttons
			list_enable[i].configure(state = "normal")
		if buttons_to_update == 0:
			list_enable[0].configure(state = "disabled")


	#window_player_creation = Toplevel(game_window)
	window_player_creation = Toplevel(root)
	window_player_creation.title("New Game")

	#create 3 frames
	#1st with radio buttons for the player to choose amount of human players
	#2nd with radio buttons for the player to choose amount of bots
	#3rd with with button "launch" 
	player_creation_human_players_frame = Frame(window_player_creation)
	player_creation_bots_frame = Frame(window_player_creation)
	player_creation_launch_frame =Frame(window_player_creation)
	player_creation_human_players_frame.pack(side="left", fill="both", expand="yes")
	player_creation_bots_frame.pack(side="left", fill="both", expand="yes")
	player_creation_launch_frame.pack(side="left", fill="both", expand="yes")

	#two vars that change based on button pressed (they just jump between numbers (0 to 8))
	#one number of humans is a var that represents the chosen number of human players
	#the second one is representing the number of bots in the game chosen by the player
	number_of_humans = IntVar()
	number_of_bots = IntVar()

	#this creates the radio buttons 8 for human players and 8 for computer players
	humans_1_radio = Radiobutton(player_creation_human_players_frame, text = "humans 1" ,variable = number_of_humans, value = 1,indicatoron=0,command = lambda:update_radio_buttons(0,bot_radio_list))
	humans_2_radio = Radiobutton(player_creation_human_players_frame, text = "humans 2" ,variable = number_of_humans, value = 2,indicatoron=0,command = lambda:update_radio_buttons(1,bot_radio_list))
	humans_3_radio = Radiobutton(player_creation_human_players_frame, text = "humans 3" ,variable = number_of_humans, value = 3,indicatoron=0,command = lambda:update_radio_buttons(2,bot_radio_list))
	humans_4_radio = Radiobutton(player_creation_human_players_frame, text = "humans 4" ,variable = number_of_humans, value = 4,indicatoron=0,command = lambda:update_radio_buttons(3,bot_radio_list))
	humans_5_radio = Radiobutton(player_creation_human_players_frame, text = "humans 5" ,variable = number_of_humans, value = 5,indicatoron=0,command = lambda:update_radio_buttons(4,bot_radio_list))
	humans_6_radio = Radiobutton(player_creation_human_players_frame, text = "humans 6" ,variable = number_of_humans, value = 6,indicatoron=0,command = lambda:update_radio_buttons(5,bot_radio_list))
	humans_7_radio = Radiobutton(player_creation_human_players_frame, text = "humans 7" ,variable = number_of_humans, value = 7,indicatoron=0,command = lambda:update_radio_buttons(6,bot_radio_list))
	humans_8_radio = Radiobutton(player_creation_human_players_frame, text = "humans 8" ,variable = number_of_humans, value = 8,indicatoron=0,command = lambda:update_radio_buttons(7,bot_radio_list))
	humans_1_radio.pack(side="top", fill="both", expand="yes")
	humans_2_radio.pack(side="top", fill="both", expand="yes")
	humans_3_radio.pack(side="top", fill="both", expand="yes")
	humans_4_radio.pack(side="top", fill="both", expand="yes")
	humans_5_radio.pack(side="top", fill="both", expand="yes")
	humans_6_radio.pack(side="top", fill="both", expand="yes")
	humans_7_radio.pack(side="top", fill="both", expand="yes")
	humans_8_radio.pack(side="top", fill="both", expand="yes")

	bots_0_radio = Radiobutton(player_creation_bots_frame, text = "bots 0" ,variable = number_of_bots, value = 0,indicatoron=0,command = lambda:update_radio_buttons(0,human_radio_list))
	bots_1_radio = Radiobutton(player_creation_bots_frame, text = "bots 1" ,variable = number_of_bots, value = 1,indicatoron=0,command = lambda:update_radio_buttons(1,human_radio_list))
	bots_2_radio = Radiobutton(player_creation_bots_frame, text = "bots 2" ,variable = number_of_bots, value = 2,indicatoron=0,command = lambda:update_radio_buttons(2,human_radio_list))
	bots_3_radio = Radiobutton(player_creation_bots_frame, text = "bots 3" ,variable = number_of_bots, value = 3,indicatoron=0,command = lambda:update_radio_buttons(3,human_radio_list))
	bots_4_radio = Radiobutton(player_creation_bots_frame, text = "bots 4" ,variable = number_of_bots, value = 4,indicatoron=0,command = lambda:update_radio_buttons(4,human_radio_list))
	bots_5_radio = Radiobutton(player_creation_bots_frame, text = "bots 5" ,variable = number_of_bots, value = 5,indicatoron=0,command = lambda:update_radio_buttons(5,human_radio_list))
	bots_6_radio = Radiobutton(player_creation_bots_frame, text = "bots 6" ,variable = number_of_bots, value = 6,indicatoron=0,command = lambda:update_radio_buttons(6,human_radio_list))
	bots_7_radio = Radiobutton(player_creation_bots_frame, text = "bots 7" ,variable = number_of_bots, value = 7,indicatoron=0,command = lambda:update_radio_buttons(7,human_radio_list))
	bots_0_radio.pack(side="top", fill="both", expand="yes")
	bots_1_radio.pack(side="top", fill="both", expand="yes")
	bots_2_radio.pack(side="top", fill="both", expand="yes")
	bots_3_radio.pack(side="top", fill="both", expand="yes")
	bots_4_radio.pack(side="top", fill="both", expand="yes")
	bots_5_radio.pack(side="top", fill="both", expand="yes")
	bots_6_radio.pack(side="top", fill="both", expand="yes")
	bots_7_radio.pack(side="top", fill="both", expand="yes")

	#for some reason when 0 bots is always pressed when program is launched therefore deselect it when game launches
	bots_0_radio.deselect()
	#lists of the radio  buttons so they can be edited (disabled or enabled)
	bot_radio_list = [bots_7_radio,bots_6_radio,bots_5_radio,bots_4_radio,bots_3_radio,bots_2_radio,bots_1_radio,bots_0_radio]
	human_radio_list = [humans_8_radio,humans_7_radio,humans_6_radio,humans_5_radio,humans_4_radio,humans_3_radio,humans_2_radio,humans_1_radio]

	#launch game button is to be pressed when the user is ready
	launch_game_button = Button(player_creation_launch_frame,text = "Launch game", command = lambda : player_creation(
	player_creation_bots_frame,player_creation_human_players_frame,player_creation_launch_frame,window_player_creation,root))
	launch_game_button.pack(side="left", fill=X, expand = True)

	window_player_creation.mainloop()

def option_load(root):
	root.withdraw()
	#hide the menu window,
	#keep looping until the player loads the game or exits the load option
	base = os.getcwd()
	path = base + "\\Saved Games\\Slot " 
	for i in range(5):
		i=i+1
		path = path + str(i)
		print("save game slot",i)
		try:
			file = open(path+"\\save info.dat")
			save_info = pickle.load(file)
			file.close()
			print(save_info)
		except:
			print("empty")
	while True:
		while True:
			try:
				#make the player choose number from 1 to 5 only or exit
				save_game_slot = int(input("Choose a Slot [1,2,3,4,5] or exit: "))
			except:
				#if player wants to exit ask confirmation
				choice = tkinter.messagebox.askquestion("Exit","You did not enter a number, do you want to exit?")
				if choice == "yes":
					#if wants to exit show the menu again and exit the function
					root.deiconify()
					return
				try:
					#check if the chosen number is valid
					if save_game_slot > 0 and save_game_slot < 6 or save_game_slot == "exit":
						#if is break the loop
						break
				except:
					print("That's not a valid location choose a number between 1 and 5")
		try:
			#try to load
			#get the files location
			base = os.getcwd()
			path = base+"\\Saved Games\\Slot "+save_game_slot
			#load players
			file = open(path+"\\players.dat")
			players = pickle.load(file)
			file.close()
			#load properties
			file = open(path+"\\all properties.dat","rb")
			all_properties = pickle.load(file)
			file.close()
			#load chest/chance lists
			file = open("chance cards","rb")
			chance_list = pickle.load(file)
			file.close()
			file.open("used chance cards","rb")
			used_chance_list = pickle.load(file)
			file.close()
			file.open("chest cards","rb")
			chest_list = pickle.load(file)
			file.close()
			file.open("used chest cards","rb")
			used_chest_list = pickle.load(file)
			file.close()
			#if loaded exit the loop
			break
		except:
			#if location contains nothing
			print("Selected location")
			#create the game with loaded files
	create_new_game(root)

def start_game(root):
	#getting the path of the program and changing it to the folder of the files.
	base = os.getcwd()
	path = base+"\\Saved Games\\Slot 0"

	#load the 3 lists to the program
	file = open(path+"\\board position list.dat","rb")
	board_position_list = pickle.load(file)
	file.close()
	file = open(path+"\\chance cards.dat","rb")
	chance_card_list = pickle.load(file)
	file.close()
	file = open(path+"\\chest cards.dat","rb")
	chest_card_list = pickle.load(file)
	file.close()
	#shuffle the cards
	random.shuffle(chance_card_list)
	random.shuffle(chest_card_list)

	#starting the turn system, 
	global current_player
	global current_turn
	current_turn = 0
	current_player = players[current_turn]

	#creating the window with the board and buttons called game
	game_window = Toplevel(root)
	game_window.title("Game")
	game_window.resizable(width=False, height=False)
	left_frame = Frame(game_window)
	left_frame.grid(row = 0)

	board = Canvas(left_frame,width = 1000, height = 1000)
	board.pack()

	#loading the board image file
	#create the board girst so it will stay in the background
	board_image = PhotoImage(file = "Images\\board 1000x1000.png")
	board.create_image(0,0, image = board_image,anchor = NW)
	#see what colours were chosen (no longer in the list) and create the pawns

	if "black" not in colours:
		black_pawn_image = PhotoImage(file = "Images\\Pawns\\Black Pawn UP.png")
		black_pawn =board.create_image(875,875, image = black_pawn_image)
	if "red" not in colours:
		red_pawn_image = PhotoImage(file = "Images\\Pawns\\Red Pawn UP.png")
		red_pawn = board.create_image(900,875, image = red_pawn_image)
	if "yellow" not in colours:
		yellow_pawn_image = PhotoImage(file = "Images\\Pawns\\Yellow Pawn UP.png")
		yellow_pawn = board.create_image(875,900, image = yellow_pawn_image)
	if "orange" not in colours:
		orange_pawn_image = PhotoImage(file = "Images\\Pawns\\Orange Pawn UP.png")
		orange_pawn = board.create_image(900,900, image = orange_pawn_image)
	if "pink" not in colours:
		pink_pawn_image = PhotoImage(file = "Images\\Pawns\\Pink Pawn UP.png")
		pink_pawn = board.create_image(875,925, image = pink_pawn_image)
	if "purple" not in colours:
		purple_pawn_image = PhotoImage(file = "Images\\Pawns\\Purple Pawn UP.png")
		purple_pawn = board.create_image(900,925, image = purple_pawn_image)
	if "green" not in colours:
		green_pawn_image = PhotoImage(file = "Images\\Pawns\\Green Pawn UP.png")
		green_pawn = board.create_image(875,950, image = green_pawn_image)
	if "blue" not in colours:
		blue_pawn_image = PhotoImage(file = "Images\\Pawns\\Blue Pawn UP.png")
		blue_pawn = board.create_image(900,950, image = blue_pawn_image)
	for player in players:
		if player.colour == "black":
			player.set_pawn(black_pawn,False,75,875)
		if player.colour == "red":
			player.set_pawn(red_pawn,False,100,875)
		if player.colour == "yellow":
			player.set_pawn(yellow_pawn,False,75,900)
		if player.colour == "orange":
			player.set_pawn(orange_pawn,False,100,900)
		if player.colour == "pink":
			player.set_pawn(pink_pawn,True,75,925)
		if player.colour == "purple":
			player.set_pawn(purple_pawn,True,100,925)
		if player.colour == "green":
			player.set_pawn(green_pawn,True,75,950)
		if player.colour == "blue":
			player.set_pawn(blue_pawn,True,100,950)


	#splitting the window into 2 frames, left is for board and right is for buttons and player info
	right_frame = Frame(game_window)
	right_frame.grid(row = 0 ,column = 1)
	top_frame = Frame(right_frame)
	top_frame.grid(column = 0)
	bottom_frame = Frame(right_frame)
	bottom_frame.grid(column = 0,row = 1)
	#load the die images.
	die_image = PhotoImage(file = "Images\\Dice\\die 6.png")
	die_1_image_label = Label(top_frame, image = die_image)
	die_2_image_label = Label(top_frame, image = die_image)
	die_1_image_label.grid(row=3)
	die_2_image_label.grid(row=3,column=1)
	#create the roll dice and end turn button, while making one active and other inactive
	button_roll_dice = Button(top_frame,text = "roll dice", fg="red" ,state = "normal" ,width = 10 ,command = lambda: roll_dice(current_player,game_window,die_1_image_label,die_2_image_label,button_roll_dice,button_end_turn,board_position_list,name_label,colour_label,position_label,money_label,chance_card_list,chest_card_list,board))
	button_roll_dice.grid(row = 2)
	button_end_turn = Button(top_frame,text = "end turn" ,fg="red" ,state = "disabled" ,width = 10 ,command = lambda: next_turn(button_roll_dice,button_end_turn,name_label,colour_label,position_label,money_label))
	button_end_turn.grid(row = 2,column = 1)

	#create the player layout for the first time
	#this contains name, colour, position, money
	name = current_player.get("name")
	colour = current_player.get("colour")
	position = "position: " + str(current_player.get("position"))
	money = "money: " + str(current_player.get("money"))
	name_label = Label(top_frame,text = name,fg = colour)
	colour_label = Label(top_frame,text = colour,fg = colour)
	position_label = Label(top_frame,text = position,fg = colour)
	money_label = Label(top_frame,text = money, fg = colour)
	name_label.grid(row = 0,sticky = (E))
	colour_label.grid(row = 0,column = 1,sticky = (W))
	position_label.grid(row = 1,sticky = (E))
	money_label.grid(row = 1,column = 1,sticky = (W))
	#create the options buttons
	#most of them are just there for now (disabled means cannot be pressed) and will be soon given functions.
	button_build = Button(top_frame,text = "Build" ,width = 10,command = lambda: property_management_window(current_player,game_window,name_label,colour_label,position_label,money_label,"Build"))
	button_sell = Button(top_frame,text = "Sell" ,width = 10, command = lambda: property_management_window(current_player,game_window,name_label,colour_label,position_label,money_label,"Sell"))
	button_morgage = Button(top_frame,text = "Mortgage",width = 10,command = lambda: property_management_window(current_player,game_window,name_label,colour_label,position_label,money_label,"Mortgage"))
	button_unmorgage = Button(top_frame,text = "Unmortgage",width = 10,command = lambda: property_management_window(current_player,game_window,name_label,colour_label,position_label,money_label,"Unmortgage"))
	button_trade = Button(top_frame,text = "Trade",state = "disabled",width = 10)
	button_options = Button(top_frame,text = "Options",state = "disabled",width = 10)
	button_quit = Button(bottom_frame,text = "Quit" , width = 20 ,command = lambda: option_quit(game_window))

	button_build.grid(row=4)
	button_sell.grid(row=4,column=1)
	button_morgage.grid(row=5)
	button_unmorgage.grid(row=5,column=1)
	button_options.grid(row=6,column=1)
	button_trade.grid(row=6)
	button_quit.grid(row=0)
	#keep the window running at all times.
	game_window.mainloop()

def save_game(players,board_position_list):
	#save game
	#make a new window
	#save game text
	#save name cannot be empty
	now = time.localtime(time.time())
	time_saving = time.strftime("%y/%m/%d %H:%M", now)
	while True:
		save_game_slot = str(input("Choose a Slot [1,2,3,4,5]: "))
		if save_game_slot > 0 or save_game_slot < 6:
			break
	base = os.getcwd()
	path = base +"\\Saved Games\\" + save_game_slot
	file = open(path+"\\save info.dat","wb")
	pickle.dump(time_saving,file)
	file.close()
	file = open(path+"\\players.dat","wb")
	pickle.dump(players,file)
	file.close()
	file = open(path+"\\board position list.dat","wb")
	pickle.dump(board_position_list,file)
	file.close()

#first actual code executed by the program
#create menu window
root = Tk()
root.title("Menu")

#create the 3 buttons (new game, load or quit)
Button(root,text = "New Game", fg = "red", bg = "white", command = lambda : option_new(root)).pack()
Button(root,text = "Load Game", fg = "red", bg = "white", command = lambda : option_load(root)).pack()
Button(root,text = "Quit Game", fg = "red", bg = "white",  command = lambda : option_quit(root)).pack()

#keep it in the loop
root.mainloop()