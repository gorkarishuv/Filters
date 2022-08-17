#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg;
            // calculate avg in floating point value
            avg = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0;
            // round up the avg and change it to byte before saving in the rgbt values
            image[i][j].rgbtRed = (BYTE)(round(avg));
            image[i][j].rgbtBlue = (BYTE)(round(avg));
            image[i][j].rgbtGreen = (BYTE)(round(avg));
        }

    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // declare an object image_copy to store intermediate values without changing the values in original image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // store new values in image_copy
            image_copy[i][j] = image[i][width - j - 1];
        }
    }
    // overwrite values in image by those in image_copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // declare an object image_copy to store intermediate values without changing the values in original image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumred = 0;
            float sumblue = 0;
            float sumgreen = 0;

            // figure out values to interae through to find the required avg
            int prv_row = i - 1;
            int next_row = i + 1;
            int prv_coloumn = j - 1;
            int next_coloumn = j + 1;
            // take care of cases when the current pixel is on the edge/corner and not sorrounded by 8 other pixels
            if (prv_row < 0)
            {
                prv_row = 0;
            }
            if (next_row >= height)
            {
                next_row = height - 1;
            }
            if (prv_coloumn < 0)
            {
                prv_coloumn = 0;
            }
            if (next_coloumn >= width)
            {
                next_coloumn = width - 1;
            }

            int count = 0;

            // interate through surrounding pixels to find the avg
            for (int k = prv_row; k <= next_row; k++)
            {
                for (int l = prv_coloumn; l <= next_coloumn; l++)
                {
                    sumred += image[k][l].rgbtRed;
                    sumblue += image[k][l].rgbtBlue;
                    sumgreen += image[k][l].rgbtGreen;
                    count ++;
                }
            }
            // divide the total sum by count to find the avg and copy it in image_copy
            image_copy[i][j].rgbtRed = (BYTE)(round(sumred / count));
            image_copy[i][j].rgbtBlue = (BYTE)(round(sumblue / count));
            image_copy[i][j].rgbtGreen = (BYTE)(round(sumgreen / count));
        }
    }
    // overwrite values in image by new updaated values in image_copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // declare an object image_copy to store intermediate values without changing the values in original image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // variavles to calculate Gx and Gy value for red, blue and green pixel value
            float sumred_x = 0;
            float sumblue_x = 0;
            float sumgreen_x = 0;

            float sumred_y = 0;
            float sumblue_y = 0;
            float sumgreen_y = 0;
            float sumred, sumblue, sumgreen;

            // figure out values to interae through to find the required avg
            int prv_row = i - 1;
            int next_row = i + 1;
            int prv_coloumn = j - 1;
            int next_coloumn = j + 1;
            // take care of boundary cases i.e, cases where no. of pixels surrounding current pixel is less than 8
            if (prv_row < 0)
            {
                prv_row = 0;
            }
            if (next_row >= height)
            {
                next_row = height - 1;
            }
            if (prv_coloumn < 0)
            {
                prv_coloumn = 0;
            }
            if (next_coloumn >= width)
            {
                next_coloumn = width - 1;
            }

            // interate through surrounding pixels to calculate the weighted avg
            for (int k = prv_row; k <= next_row; k++)
            {
                for (int l = prv_coloumn; l <= next_coloumn; l++)
                {
                    // calculate the factor each pixel should be multiplied with before adding to the sum
                    int x, y;
                    x = l - j;
                    y = k - i;
                    if (x == 0)
                    {
                        y *= 2;
                    }
                    if (y == 0)
                    {
                        x *= 2;
                    }
                    sumred_x += x * image[k][l].rgbtRed;
                    sumblue_x += x * image[k][l].rgbtBlue;
                    sumgreen_x += x * image[k][l].rgbtGreen;

                    sumred_y += y * image[k][l].rgbtRed;
                    sumblue_y += y * image[k][l].rgbtBlue;
                    sumgreen_y += y * image[k][l].rgbtGreen;

                }
            }
            // finally calculate the final value for each pixel by taking sqare root of Gx^2 + Gy^2
            sumred = sqrt(pow(sumred_x, 2) + pow(sumred_y, 2));
            sumblue = sqrt(pow(sumblue_x, 2) + pow(sumblue_y, 2));
            sumgreen = sqrt(pow(sumgreen_x, 2) + pow(sumgreen_y, 2));

            // cap the values to 255
            if (sumred > 255)
            {
                sumred = 255;
            }
            if (sumblue > 255)
            {
                sumblue = 255;
            }
            if (sumgreen > 255)
            {
                sumgreen = 255;
            }
            // save the weighted avg values in image_copy
            image_copy[i][j].rgbtRed = (BYTE)(round(sumred));
            image_copy[i][j].rgbtBlue = (BYTE)(round(sumblue));
            image_copy[i][j].rgbtGreen = (BYTE)(round(sumgreen));
        }
    }
    // overwrite values in image by new values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    return;
}
