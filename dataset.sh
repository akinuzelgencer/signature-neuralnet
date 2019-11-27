mkdir -p "dataset"
cd "dataset"

if [ ! -f  "./dataset.zip" ]
then 
	echo "Downloading dataset (~250MB)"
	curl "http://www.iapr-tc11.org/dataset/4NSigComp2012/Testset_4NsigComp2012.zip" > "dataset.zip"
fi

unzip -P "I hereby accept the 4NsigComp2012 disclaimer." dataset.zip
mv Testset_4NsigComp2012/* .
rmdir Testset_4NsigComp2012
rename "s/Correct_Anwsers_Key\.xls/Answers.xls/" "Correct_Anwsers_Key.xls"

rename 's/Ref/train/' Ref
rename 's/Questioned/test/' Questioned

cd train
rm "A1/K001.PNG" "A1/K002.PNG" "A1/K003.PNG" "A1/K004.PNG" "A1/K005.PNG" "A2/K015.PNG"
rename 's/.+/our $i; sprintf("Amy-%3.3d.png", 1+$i++)/e' A1/*.PNG
rename 's/.+/our $i; sprintf("Bob-%3.3d.png", 1+$i++)/e' A2/*.PNG
rename 's/.+/our $i; sprintf("Eva-%3.3d.png", 1+$i++)/e' A3/*.PNG
rmdir A1 A2 A3
cd ..

cd test
rm "A2/Q034test.png"
rename 's/.+/our $i; sprintf("Amy-%3.3d.png", 1+$i++)/e' A1/*.PNG
rename 's/.+/our $i; sprintf("Bob-%3.3d.png", 1+$i++)/e' A2/*.PNG
rename 's/.+/our $i; sprintf("Eva-%3.3d.png", 1+$i++)/e' A3/*.PNG
rmdir A1 A2 A3
cd ..

mogrify -monitor */*.png