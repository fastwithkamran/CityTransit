# CityTransit

Our DSA Project, aim to make transportation more feasible for Public!!

## R&D

The project flow is..

Extract data in CSV format contains coordinates of the places and roads in Karachi from [OverPass](https://overpass-turbo.eu/)

Input from the user PickUp, Destination, and Vehicle

Fetch [TomTom API](https://docs.tomtom.com) to update the CSV

Use Dijsktra to find the shortest route (result depends on Time and Distance factor)

Fetch Petrol API to get latest petrol prices

Output the cost, ETA based on TomTom API, shortest route
