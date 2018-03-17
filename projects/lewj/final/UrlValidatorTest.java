/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

import junit.framework.TestCase;

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1739358 $
 */
public class UrlValidatorTest extends TestCase {
   public UrlValidatorTest(String testName) {
      super(testName);
   }

   public void testManualTest() throws FileNotFoundException, UnsupportedEncodingException {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   PrintWriter writer_true = new PrintWriter("correct-true.txt", "UTF-8");
	   PrintWriter writer_false = new PrintWriter("correct-false.txt", "UTF-8");
	   PrintWriter writer_bad = new PrintWriter("incorrect.txt", "UTF-8");
	   	   
	   assertTrue(urlVal.isValid("https://www.google.com"));
	   assertTrue(urlVal.isValid("https://www.bing.com"));
	   //assertTrue(urlVal.isValid("https://www.google.com/~lewj"));
	   
	   assertTrue(testUrlScheme[0].valid);
	   assertEquals(testUrlScheme[0].item, "http://");
	   
	   for (int i = 0; i < testUrlScheme.length; i++) {
		   for (int j = 0; j < testUrlAuthority.length; j++) {
			   for (int k = 0; k < testUrlPort.length; k++) {
				   for (int m = 0; m < testPath.length; m++) {
					   for (int n = 0; n < testUrlQuery.length; n++) {
						   StringBuilder testUrl = new StringBuilder();
						   boolean urlValid = true;
						   
						   testUrl.append(testUrlScheme[i].item);
						   urlValid = urlValid && testUrlScheme[i].valid;
						   
						   testUrl.append(testUrlAuthority[j].item);
						   urlValid = urlValid && testUrlAuthority[j].valid;
						   
						   testUrl.append(testUrlPort[k].item);
						   urlValid = urlValid && testUrlPort[k].valid;
						   
						   testUrl.append(testPath[m].item);
						   urlValid = urlValid && testPath[m].valid;
						   
						   testUrl.append(testUrlQuery[n].item);
						   urlValid = urlValid && testUrlQuery[n].valid;
						   
						   if (urlVal.isValid(testUrl.toString()) == urlValid && urlValid == true) {
							   writer_true.println(testUrl);
						   }
						   else if (urlVal.isValid(testUrl.toString()) == urlValid && urlValid == false) {
							   writer_false.println(testUrl);
						   }
						   else {
							   writer_bad.print(testUrl);
							   writer_bad.print(" | expected: ");
							   writer_bad.print(urlValid);
							   writer_bad.print(", urlValidator: ");
							   writer_bad.println(urlVal.isValid(testUrl.toString()));
						   }
						  
						 
					   }
				   }
			   }
		   }
	   }
	  
	   writer_true.close();
	   writer_false.close();
	   writer_bad.close();
	   
   }






   //-------------------- Test data for creating a composite URL
   /**
    * The data given below approximates the 4 parts of a URL
    * <scheme>://<authority><path>?<query> except that the port number
    * is broken out of authority to increase the number of permutations.
    * A complete URL is composed of a scheme+authority+port+path+query,
    * all of which must be individually valid for the entire URL to be considered
    * valid.
    */
   ResultPair[] testUrlScheme = {new ResultPair("http://", true),
                               new ResultPair("ftp://", true),
                               new ResultPair("h3t://", true),
                               new ResultPair("3ht://", false),
                               new ResultPair("http:/", false),
                               new ResultPair("http:", false),
                               new ResultPair("http/", false),
                               new ResultPair("://", false),
                               new ResultPair("", true)};

   ResultPair[] testUrlAuthority = {new ResultPair("www.google.com", true),
                                  new ResultPair("go.com", true),
                                  new ResultPair("go.au", true),
                                  new ResultPair("0.0.0.0", true),
                                  new ResultPair("255.255.255.255", true),
                                  new ResultPair("256.256.256.256", false),
                                  new ResultPair("255.com", true),
                                  new ResultPair("1.2.3.4.5", false),
                                  new ResultPair("1.2.3.4.", false),
                                  new ResultPair("1.2.3", false),
                                  new ResultPair(".1.2.3.4", false),
                                  new ResultPair("go.a", false),
                                 new ResultPair("go.a1a", false),
                                  new ResultPair("go.1aa", false),
                                  new ResultPair("aaa.", false),
                                  new ResultPair(".aaa", false),
                                  new ResultPair("aaa", false),
                                  new ResultPair("", false)
   };
   ResultPair[] testUrlPort = {new ResultPair(":80", true),
                             new ResultPair(":65535", true),
                             new ResultPair(":0", true),
                             new ResultPair("", true),
                             new ResultPair(":-1", false),
                            new ResultPair(":65636",false),
                             new ResultPair(":65a", false)
   };
   ResultPair[] testPath = {new ResultPair("/test1", true),
                          new ResultPair("/t123", true),
                          new ResultPair("/$23", true),
                          new ResultPair("/..", false),
                          new ResultPair("/../", false),
                          new ResultPair("/test1/", true),
                          new ResultPair("", true),
                          new ResultPair("/test1/file", true),
                          new ResultPair("/..//file", false),
                          new ResultPair("/test1//file", false),
                          new ResultPair("/~mpython", true)
   };
   //Test allow2slash, noFragment
   ResultPair[] testUrlPathOptions = {new ResultPair("/test1", true),
                                    new ResultPair("/t123", true),
                                    new ResultPair("/$23", true),
                                    new ResultPair("/..", false),
                                    new ResultPair("/../", false),
                                    new ResultPair("/test1/", true),
                                    new ResultPair("/#", false),
                                    new ResultPair("", true),
                                    new ResultPair("/test1/file", true),
                                    new ResultPair("/t123/file", true),
                                    new ResultPair("/$23/file", true),
                                    new ResultPair("/../file", false),
                                    new ResultPair("/..//file", false),
                                    new ResultPair("/test1//file", true),
                                    new ResultPair("/#/file", false)
   };

   ResultPair[] testUrlQuery = {new ResultPair("?action=view", true),
                              new ResultPair("?action=edit&mode=up", true),
                              new ResultPair("", true)
   };

   Object[] testUrlParts = {testUrlScheme, testUrlAuthority, testUrlPort, testPath, testUrlQuery};
   Object[] testUrlPartsOptions = {testUrlScheme, testUrlAuthority, testUrlPort, testUrlPathOptions, testUrlQuery};
   int[] testPartsIndex = {0, 0, 0, 0, 0};

   //---------------- Test data for individual url parts ----------------
   ResultPair[] testScheme = {new ResultPair("http", true),
                            new ResultPair("ftp", false),
                            new ResultPair("httpd", false),
                            new ResultPair("telnet", false)};


}
