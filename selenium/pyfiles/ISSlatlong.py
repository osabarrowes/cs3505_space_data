import time
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
CHROMEDRIVER_PATH = '/usr/local/bin/chromedriver'
WINDOW_SIZE = "1920,1080"
chrome_options = Options()
chrome_options.add_argument("--headless")
chrome_options.add_argument("--window-size=%s" % WINDOW_SIZE)
chrome_options.add_argument('--no-sandbox')
driver = webdriver.Chrome(executable_path=CHROMEDRIVER_PATH,
                          options=chrome_options
                         )
driver.get("http://wsn.spaceflight.esa.int/iss/index_portal.php")
time.sleep(20)
Latitude = driver.find_element_by_xpath("/html/body/center/div[2]/div[1]/div[3]/table/tbody/tr/td[1]/div[2]")
Longitude = driver.find_element_by_xpath("/html/body/center/div[2]/div[1]/div[3]/table/tbody/tr/td[3]/div[2]")
str1 = r"/dataStorage/latlong" + str(time.time()) + ".txt"
fileObject = open(str1, "w")
fileObject.write(Latitude.text + " " + Longitude.text)
fileObject.close()
driver.close()
