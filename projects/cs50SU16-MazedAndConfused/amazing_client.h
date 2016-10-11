/* ************************************************************************** */
/* File: amazing_client.h
 *
 * Project name: CS50 Amazing Project
 * Component name: amazing client file
 *
 * Primary Author: Mazed and Confused
 * Date Created: August 2016
 *
 * Last Update by: Mazed and Confused - Sunday, 28 August 2016
 *
 * Header file for the amazing_client in the Amazing Project.
 *
 */

/* *****************************************************************************
* Name: client
*
* Description:  Function to drive the behavior of each individual avatar. To be run
*				concurrently with multiple avatars to solve the maze
*
* Input: arg : a pointer to a struct of information to pass to the avatar
*
* Return: NULL void pointer
*
*/
void *client(void *arg);
