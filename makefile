make:
	gcc -o theme_park main.c FinalHauntedHouse.c FoodCourtFinal.c rides.c image_viewer.c -lpthread `pkg-config --cflags --libs gtk+-3.0`

	
