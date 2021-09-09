# Run tests using flight data from the MATeF-2 flight

# Simulate flight from beginning
./predictor < test_input0.txt > test_out0.txt
./prediction_to_kml.sh test_out0.txt > test_out0.kml
# Simulate from 25% of flight
./predictor < test_input25.txt > test_out25.txt
./prediction_to_kml.sh test_out25.txt > test_out25.kml
# Simulate from 50% of flight
./predictor < test_input50.txt > test_out50.txt
./prediction_to_kml.sh test_out50.txt > test_out50.kml
# Simulate from 75% of flight
./predictor < test_input75.txt > test_out75.txt
./prediction_to_kml.sh test_out75.txt > test_out75.kml
